import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';
import { NotFoundError } from '@app/core/errors/notFoundError';
import { BadInput } from '@app/core/errors/badInput';
import { AppError } from '@app/core/errors/appError';

const routes = {
  Employees : '/employees'
};

@Injectable({
  providedIn: 'root'
})
export class EmployeesHttpService {

  constructor(private httpClient: HttpClient) { }

  getEmployees(): Promise<object>  {
    return this.httpClient.get(routes.Employees).pipe(
        catchError(this.errorHandler)
    ).toPromise();
  }

  updateName(nodeAddress: string, newName: string): Promise<object>  {
    return this.httpClient.put(`${routes.Employees}`, 
        {
            nodeAddress: nodeAddress,
            name: newName
        }).pipe(
        catchError(this.errorHandler)
    ).toPromise();
  }

    private errorHandler (err: Response) {
        if (err.status === 404) {
            return throwError(new NotFoundError(err));
        } 
        else if (err.status === 400) {
            return throwError(new BadInput(err));
        } 
        else {
            return throwError(new AppError(err));
        }
    }
}
