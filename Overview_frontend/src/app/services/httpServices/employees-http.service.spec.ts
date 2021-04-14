import { TestBed, inject } from '@angular/core/testing';

import { EmployeesHttpService } from '@app/services/httpServices/employees-http.service';

import { Logger } from '@app/core/logger.service';
const log = new Logger('EmployeesHttpService');

xdescribe('EmployeesHttpService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [EmployeesHttpService]
    });
  });

  it('should be created', inject([EmployeesHttpService], (service: EmployeesHttpService) => {
    expect(service).toBeTruthy();
  }));
});
