import { Injectable } from '@angular/core';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { Employee } from './employees.service';

@Injectable({
    providedIn: 'root'
})
export class HealthStatusService {

    // private HeartRateAggre = new Map();
    // private meanSample: number = 3;

    constructor(
        private employeesService: EmployeesService
    ){ 

    }

    public evaluateHeartrate(hearRate: number): number {
        if      (hearRate>130) return 5
        else if (hearRate>110) return 4
        else if (hearRate>90)  return 3
        else if (hearRate>70)  return 2
        else return 1
    }

    public getLastHeartRate(nodeAddress: string): number {
        let employee: Employee;
        employee = this.employeesService.employees.findEmployee(nodeAddress);
        let timeserie = employee.getLastTimeserie();
        return timeserie.heartRate;
    }

    public getHealthStatus(nodeAddress: string): number {
        return this.evaluateHeartrate(this.getLastHeartRate(nodeAddress));
    }
}
