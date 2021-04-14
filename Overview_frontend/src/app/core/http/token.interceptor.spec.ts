import { TestBed } from '@angular/core/testing';
import {
    HttpClientTestingModule,
    HttpTestingController,
} from '@angular/common/http/testing';
import { HTTP_INTERCEPTORS } from '@angular/common/http';
import { EmployeesHttpService } from '@app/services/httpServices/employees-http.service';
import { TokenInterceptor } from '@app/core/http/token.interceptor';

import { Logger } from '@app/core/logger.service';
const log = new Logger('TokenInterceptor');

describe(`TokenIn`, () => {
    let service: EmployeesHttpService;
    let httpMock: HttpTestingController;

    beforeEach(() => {
        TestBed.configureTestingModule({
            imports: [HttpClientTestingModule],
            providers: [
                EmployeesHttpService,
                {
                    provide: HTTP_INTERCEPTORS,
                    useClass: TokenInterceptor,
                    multi: true,
                },
            ],
        });

        service = TestBed.get(EmployeesHttpService);
        httpMock = TestBed.get(HttpTestingController);
    });

    it('should add an Authorization header', () => {
        service.getEmployees().then(response => {
            expect(response).toBeTruthy();
        });

        const httpRequest = httpMock.expectOne('/api/timeseries');

        // log.debug(httpRequest.request.headers.getAll('authorization'));

        expect(httpRequest.request.headers.has('authorization'));
    });
});
