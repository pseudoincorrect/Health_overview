import { Injectable } from '@angular/core';
import { TimeseriesHttpService } from '@app/services/httpServices/timeseries-http.service';
import { EmployeesHttpService } from '@app/services/httpServices/employees-http.service';
import * as moment from 'moment';
import { timestamp } from 'rxjs/operators';
import { AuthenticationService } from '@app/core/authentication/authentication.service';

@Injectable({
    providedIn: 'root'
})
export class EmployeesService {
    private _employees: Employees;
    public isLoading: boolean;

    constructor(
        private authService: AuthenticationService,
        private timeseriesHttpService: TimeseriesHttpService,
        private employeesHttpService: EmployeesHttpService
    ) { 
        this.isLoading = false;
    }

    get employees () { return this._employees; }

    public async init() {
        await this.getAllTimeseries();
    }

    public async getAllTimeseries () {
        let dateFrom: moment.Moment  = moment();
        dateFrom.subtract(3, 'year');

        this._employees = null;
        this._employees = new Employees();
        let timeseries = await this.timeseriesHttpService.getEmployeesDataFromDate(dateFrom.unix());

        this._employees.updateEmployeesData(timeseries);

        let employees = await this.employeesHttpService.getEmployees();
        this._employees.updateEmployeesNames(employees);
    }

    public async getAllTimeseriesfrom (timeStamp: number = null) {
        let timeseries: any;

        timeseries = await this.timeseriesHttpService.getEmployeesDataFromDate(timeStamp);

        this._employees.updateEmployeesData(timeseries);

        let employees = await this.employeesHttpService.getEmployees();
        this._employees.updateEmployeesNames(employees);
    }

    public refresh(): void {
        if (this.authService.isAuthenticated()) {
            if (this._employees !== undefined) {
                this.getAllTimeseriesfrom(this.employees.lastUpdated);
            }
            else {
                this.isLoading = true;
                this.init().then( () => this.isLoading = false );
            }
        }
    }

    // public getSingleEmployee (nodeAddress: string): any {
    //     if (this._employees) {
    //         return this._employees.findEmployee(nodeAddress);
    //     } else {
    //         return null;
    //     }
    // }

    public getSingleEmployeeData (nodeAddress: string): any {
        if (this._employees) {
            return this._employees.findEmployee(nodeAddress);
        } else {
            return null;
        }
    }

    public getAllEmployeeData (nodeAddress: string): any {
        if (this._employees) {
            return this._employees.findEmployee(nodeAddress);
        } else {
            return null;
        }
    }

    public updateEmployeeName(nodeAddress: string, newName: string) {
        for (let employee of this.employees.employees) {
            if (employee.nodeAddress === nodeAddress) {
                employee.name = newName;
            }
        }
        this.employeesHttpService.updateName(nodeAddress, newName);
    }
}

export class Employees {
    private _employees: Employee[];
    private _lastUpdatedEmployee: string;
    private _lastUpdated: number;

    constructor() {
        this._lastUpdated = 0;
        this._lastUpdatedEmployee = '';
    }

    get lastUpdated () { return this._lastUpdated; }
    get lastUpdatedEmployee () { return this._lastUpdatedEmployee; }
    get employees () { return this._employees; }

    private setLastUpdated() {
        for (const emp of this._employees) {
            if (emp.lastUpdated > this._lastUpdated) {
                this._lastUpdatedEmployee = emp.nodeAddress;
                this._lastUpdated = emp.lastUpdated;
            }
        }
    }

    public updateEmployeesData(timeseries: any) {
        if (timeseries == null) { return; }

        if (this._employees == null) { this._employees = []; }
        for (let timeserie of timeseries) {
            let employee = this.findEmployee(timeserie.nodeAddress);
            if (!employee) {
                employee = this.createEmployee(timeserie.nodeAddress);
            }
            employee.addTimeSerie(timeserie);
        }
        this.setLastUpdated();
    }

    private createEmployee (nodeAddress: string) {
        const employeeData = new Employee(nodeAddress);
        this._employees.push(employeeData);
        return this.findEmployee(nodeAddress);
    }

    public findEmployee (nodeAddress: string): Employee {
        if (!this._employees) {
            return null;
        } else {
            return this._employees.find(
                function(emp: Employee) {
                    return emp.nodeAddress === nodeAddress;
                }
            );
        }
    }

    public updateEmployeesNames(employeesSerie: any) {
        for (let employee of employeesSerie) {
            let emp = this.findEmployee(employee.nodeAddress);
            if (emp) {
                emp.name = employee.name;
            }
        }
    }

    public getDetailsFrom(nodeAddress: string, timeStampFrom: number): any {
        const emp: Employee = this.findEmployee(nodeAddress);
        let dataChart = new DataChart();
        // loop on all timeseries of an employee
        for (const timeStamp in emp.timeseries) {
            // check whether the timeserie need to be displayed
            if (Number(timeStamp) > timeStampFrom) {
                // format the data to insert
                let fTimestamp = moment.unix(Number(timeStamp) / 1000).format('HH:mm');
                let temp = Number(emp.timeseries[timeStamp].temperature);
                let humid = Number(emp.timeseries[timeStamp].humidity);
                let heartR = Number(emp.timeseries[timeStamp].heartRate);
                // if first timeserie to be inserted
                if (dataChart.timeStamp.length === 0) {
                    dataChart.timeStampFormatted.push(fTimestamp);
                    dataChart.temperature.push(temp);
                    dataChart.timeStamp.push(Number(timeStamp));
                    dataChart.heartRate.push(heartR);
                    dataChart.humidity.push(humid);
                }
                // simple sorting
                // insert the the timeseries by timestamp (break loop after insertion)
                for (let i = 0; i < dataChart.timeStamp.length; i++) {
                    // if the timestamp is superior to the current index or is last
                    if (Number(timeStamp) < dataChart.timeStamp[i]) {
                        dataChart.timeStampFormatted.splice(i, 0, fTimestamp);
                        dataChart.temperature.splice(i, 0, temp);
                        dataChart.timeStamp.splice(i, 0, Number(timeStamp));
                        dataChart.heartRate.splice(i, 0, heartR);
                        dataChart.humidity.splice(i, 0, humid);
                        break;

                    } if (i === dataChart.timeStamp.length - 1) {
                        dataChart.timeStampFormatted.push(fTimestamp);
                        dataChart.temperature.push(temp);
                        dataChart.timeStamp.push(Number(timeStamp));
                        dataChart.heartRate.push(heartR);
                        dataChart.humidity.push(humid); 
                        break;
                    }

                    
                }
            }
        }
        return dataChart;
    }
}

export class Employee {
    private _nodeAddress: string;
    private _name: string;
    private _timeseries: {};
    private _lastUpdated: number;

    constructor(nodeAddress: string) {
        this._nodeAddress = nodeAddress;
        this._timeseries = {};
    }

    set name (name: string) { this._name = name; }
    get name () { return this._name; }
    get nodeAddress (): string { return this._nodeAddress; }
    get lastUpdated () { return this._lastUpdated; }
    get timeseries () { return this._timeseries; }

    public addTimeSerie(timeserie: any) {
        let timeStamp: number = Number(timeserie.timeStamp);
        this._timeseries[timeStamp] = Object.assign({}, timeserie.timeSerie);
        this.setLastUpdated(timeserie);
        this.removeExcess();
    }

    public setLastUpdated(timeserie: any) {
        if (this._timeseries) {
            if (!this.lastUpdated || timeserie.timeStamp > this._lastUpdated) {
                this._lastUpdated = timeserie.timeStamp;
            }
        }
    }

    public getLastTimeserie() {
        if (this._lastUpdated) {
            return this._timeseries[this._lastUpdated.toString()];
        }
    }

    private removeExcess() {
        while (1) {
            // sort timestamps 
            let timestampKeys = Object.keys(this.timeseries).sort();
            if (timestampKeys.length < 55) {
                break;
            }
            // remove the oldest timestamp
            delete this._timeseries[timestampKeys[0]];
        }
    }
}

export class DataChart {
    timeStamp: number[];
    timeStampFormatted: string[];
    temperature: number[];
    humidity: number[];
    heartRate: number[];

    constructor() {
        this.timeStamp = [];
        this.timeStampFormatted = [];
        this.temperature = [];
        this.humidity = [];
        this.heartRate = [];
    }
}
