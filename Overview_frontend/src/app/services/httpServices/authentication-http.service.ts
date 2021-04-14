import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { AppError } from '@app/core/errors/appError';
import { NotFoundError } from '@app/core/errors/notFoundError';
import { BadInput } from '@app/core/errors/badInput';

const routes = {
    Authentication : '/authentication',
    GetCredentials : '/authentication/gatewaytoken',
    GetDemoToken : '/authentication/demonstration'
};

@Injectable({
    providedIn: 'root'
})
export class AuthenticationHttpService {

    constructor(private httpClient: HttpClient) { }

    postAuthenticate(login: any): Observable<object> {
        return this.httpClient.post<any>(routes.Authentication, login).pipe(
            map(token => token.token),
            catchError(this.errorHandler)
        );
    }

    getGatewayCredentials(): Promise<string> {
        return this.httpClient.get<any>(routes.GetCredentials).pipe(
            map(token => token.gatewayToken),
            catchError(this.errorHandler)
        ).toPromise();
    }

    getDemoCredentials(): Observable<object> {
        return this.httpClient.get<any>(routes.GetDemoToken).pipe(
            map(token => token.demoToken),
            catchError(this.errorHandler)
        );
    }

    private errorHandler(err: Response) {
        console.log(err, 'error in auth-http.service');
        
       if (err.status === 404) {
            return throwError(new NotFoundError(err));
        } else if (err.status === 400) {
            return throwError(new BadInput(err));
        } else {
            return throwError(new AppError(err));
        }
    }
}

// let observable: Observable<User[]> = this.http.get(uri)
//     .map((response: Response) => response.json())
//     .map((json: { _embedded: { users: User[] } }) => json._embedded.users));

// let observable: Observable<User[]> = this.httpClient.get  <_embedded: { users: User[]}>    (uri)
//     .map((json) => json._embedded.users));
//     .catch(this.handleError);
