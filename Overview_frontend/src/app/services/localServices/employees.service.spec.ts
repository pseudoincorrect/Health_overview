import { TestBed, getTestBed } from '@angular/core/testing';
import { EmployeesService, Employee } from '@app/services/localServices/employees.service';
import { Logger } from '@app/core/logger.service';
import { TimeseriesHttpService } from '@app/services/httpServices/timeseries-http.service';
import { EmployeesHttpService } from '@app/services/httpServices/employees-http.service';

const log = new Logger('employeeService');

describe('EmployeesService', () => {    
    let injector: TestBed;
    let service: EmployeesService;

    class TimeseriesHttpServiceMock {
        public getEmployeesDataFromDate(date: Number): object {
             return [
                {
                    '_id': '5b71c6162e9753d6053de26e',
                    'company': 'company_1',
                    'nodeAddress': 'address_19',
                    'timeStamp': 1534182934098,
                    'timeSerie': {
                        'temperature': 30,
                        'humidity': 33,
                        'heartRate': 80
                    }
                },
                {
                    '_id': '5b71c6162e9753d6053de26f',
                    'company': 'company_1',
                    'nodeAddress': 'address_2',
                    'timeStamp': 1534182934098,
                    'timeSerie': {
                        'temperature': 31,
                        'humidity': 28,
                        'heartRate': 90
                    },
                },
                {
                    '_id': '5b71c6162e9753d6053de26g',
                    'company': 'company_1',
                    'nodeAddress': 'address_2',
                    'timeStamp': 1534182934222,
                    'timeSerie': {
                        'temperature': 11,
                        'humidity': 22,
                        'heartRate': 99
                    },
                }
            ]; 
        }
    }
    
    class EmployeesHttpServiceMock {
        public getEmployees(): object { 
            return [
                {
                    'nodeAddress': 'address_19',
                    'name': 'name_1',
                },
                {
                    'nodeAddress': 'address_2',
                    'name': 'name_2',
                }
            ];
        }
    }
    
    beforeEach(() => {
        TestBed.configureTestingModule({
            providers: [
                EmployeesService,
                { provide: TimeseriesHttpService, useClass: TimeseriesHttpServiceMock },
                { provide: EmployeesHttpService, useClass: EmployeesHttpServiceMock },
            ]
        });

        injector = getTestBed();
        service = injector.get(EmployeesService);
    });
    
    it('should return the {a : 1}', () => {
        service.getAllTimeseries();
    });

    it('should return a single employee', () => {
        service.getAllTimeseries();
        let emp: Employee =  service.employees.findEmployee('address_2');
    });

});
