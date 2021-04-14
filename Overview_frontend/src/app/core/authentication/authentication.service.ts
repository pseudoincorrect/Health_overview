import { Injectable } from '@angular/core';
import { Observable, of, throwError} from 'rxjs';
import { AuthenticationHttpService } from '@app/services/httpServices/authentication-http.service';
import { map, catchError } from 'rxjs/operators';
import { BadInput } from '@app/core/errors/badInput';
import * as jwt_decode from 'jwt-decode';

export interface Credentials {
    // Customize received credentials here
    username: string;
    token: string;
}

export interface DecodedToken {
    _id: string;
    userType: string;
    company: string;
}

export interface LoginContext {
    username: string;
    password: string;
    remember?: boolean;
}

const credentialsKey = 'credentials';


// Provides a base for authentication workflow.
// The Credentials interface as well as login/logout methods should be replaced with proper implementation.

@Injectable()
export class AuthenticationService {
    
    private _credentials: Credentials | null;
    private _role: string | null;

    constructor( private authHttp: AuthenticationHttpService) {
        const savedCredentials = sessionStorage.getItem(credentialsKey) || localStorage.getItem(credentialsKey);
        if (savedCredentials) {
            this._credentials = JSON.parse(savedCredentials);
            let decodedToken: DecodedToken = jwt_decode(this._credentials.token);
            this._role = decodedToken.userType;
        }
    }
    
    
    // Authenticates the user.
    // @param {LoginContext} context The login parameters.
    // @return {Observable<Credentials>} The user credentials.
    login(context: LoginContext): Observable<Credentials> {
        let request =  {
            email: context.username,
            password: context.password
        };

        return this.authHttp.postAuthenticate(request).pipe(
            map( (token) => {
                let tokenstr: string = token.toString();
                
                const data = {
                    username: context.username,
                    token: tokenstr
                };
                this.setCredentials(data, context.remember);
                return (data);
            } ),
            catchError( function (err: Response) {
                console.log('error in auth.service', err);
                return throwError(new BadInput(err));
            })
        );
    }

    // Logs out the user and clear credentials.
    // @return {Observable<boolean>} True if the user was logged out successfully.
    logout(): Observable<boolean> {
        // Customize credentials invalidation here
        this.setCredentials();
        return of(true);
    }
    
    // Checks is the user is authenticated.
    // @return {boolean} True if the user is authenticated.
    isAuthenticated(): boolean {
        return !!this.credentials;
    }
    
    isManager(): boolean {
        return (this._role === 'manager');
    }
    
    loginDemo(): Observable<Credentials>  {
        return this.authHttp.getDemoCredentials().pipe(
            map( (token) => {
                let tokenstr: string = token.toString();
                const data = {
                    username: 'Mr. Guy',
                    token: tokenstr
                };
                this.setCredentials(data, false);
                return (data);
            } ),
            catchError( function (err: Response) {
                console.log('error in auth.service', err);
                return throwError(new BadInput(err));
            })
        );
    }

    // Gets the user credentials.
    // @return {Credentials} The user credentials or null if the user is not authenticated.
    get credentials(): Credentials | null {
        return this._credentials;
    }
        
    // Sets the user credentials.
    // The credentials may be persisted across sessions by setting the `remember` parameter to true.
    // Otherwise, the credentials are only persisted for the current session.
    // @param {Credentials=} credentials The user credentials.
    // @param {boolean=} remember True to remember credentials across sessions.
    
    private setCredentials(credentials?: Credentials, remember?: boolean) {
        this._credentials = credentials || null;
        
        if (credentials) {
            const storage = remember ? localStorage : sessionStorage;
            storage.setItem(credentialsKey, JSON.stringify(credentials));
        } else {
            sessionStorage.removeItem(credentialsKey);
            localStorage.removeItem(credentialsKey);
        }
    }

}
