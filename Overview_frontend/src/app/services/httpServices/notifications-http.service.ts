import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';
import { NotFoundError } from '@app/core/errors/notFoundError';
import { BadInput } from '@app/core/errors/badInput';
import { AppError } from '@app/core/errors/appError';

const routes = {
    PostNotifications : '/notifications/fromManager', 
    getNotifications : '/notifications/fromGateway'
  };

@Injectable({
  providedIn: 'root'
})
export class NotificationsHttpService {

    constructor(private httpClient: HttpClient) { }

    postNotification(nodeAddress: string): Promise<object> {
        return this.httpClient.post(routes.PostNotifications,
        {
                nodeAddress: nodeAddress,
                notifMessage: 'G_N_NOTIF'
        }).pipe(
            catchError(this.errorHandler)
        ).toPromise();
    }
    
    getNotifications(): Promise<object> {
        return this.httpClient.get(routes.getNotifications).pipe(
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
