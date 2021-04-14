import { TestBed, getTestBed, fakeAsync } from '@angular/core/testing';
import { TimeseriesHttpService } from '@app/services/httpServices/timeseries-http.service';
import {
    HttpClientTestingModule,
    HttpTestingController
} from '@angular/common/http/testing';
import { Logger } from '@app/core/logger.service';

const log = new Logger('timeserieshttp');

describe(`TimeseriesHttpService`,  () => {
    let service: TimeseriesHttpService;
    let httpMock: HttpTestingController;
    let injector: TestBed;

    beforeEach( () => {
        TestBed.configureTestingModule({
            imports: [HttpClientTestingModule],
            providers: [TimeseriesHttpService]
        });

        injector = getTestBed();
        httpMock = injector.get(HttpTestingController);
        service = injector.get(TimeseriesHttpService);
    });

    afterEach(() => {
        httpMock.verify();
    });

    it('should be created', () => {
        expect(service).toBeTruthy();
    });

    it('should return a array of latest timeseries per employee', fakeAsync (() => {
        let timeseries;
        const response = [
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
            }
        ];

        service.getEmployeesDataRecents()
            .subscribe( (res: any) => {
                timeseries = res;
                expect(res.length).toBe(response.length);
                expect(res).toEqual(response);
            });

        const req = httpMock.expectOne(`/api/timeseries`);
        expect(req.request.method).toBe('GET');
        req.flush(response);
    }));
});
