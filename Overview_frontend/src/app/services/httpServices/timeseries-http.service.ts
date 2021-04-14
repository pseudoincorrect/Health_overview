import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';
import { AppError } from '@app/core/errors/appError';
import { NotFoundError } from '@app/core/errors/notFoundError';
import { BadInput } from '@app/core/errors/badInput';

const routes = {
    Timeseries : '/timeseries'
};

@Injectable({
    providedIn: 'root'
})
export class TimeseriesHttpService {

    constructor(private httpClient: HttpClient) { }

    getEmployeesDataFromDate(timeLow: Number): Promise<object> {
        const params = new HttpParams().set('timeLow', timeLow.toString());
        
        return this.httpClient.get(routes.Timeseries, {params: params}).pipe(
            catchError(this.errorHandler)
        ).toPromise();
    }

    // getSingleEmployeeDataFromDate(
    //         nodeAddress: Date, timeLow: Number): Observable<object> {
    //     let Params = new HttpParams();
    //     Params = Params.append('timeLow', timeLow.toString());
    //     Params = Params.append('nodeAddress', nodeAddress.toString());

    //     return this.httpClient.get(routes.Timeseries, {params: Params}).pipe(
    //         catchError(this.errorHandler)
    //     );
    // }

    private errorHandler (err: Response) {
       if (err.status === 404) {
            return throwError(new NotFoundError(err));
        } else if (err.status === 400) {
            return throwError(new BadInput(err));
        } else {
            return throwError(new AppError(err));
        }
    }
}
