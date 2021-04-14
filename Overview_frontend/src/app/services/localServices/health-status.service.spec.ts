import { TestBed, inject } from '@angular/core/testing';

import { HealthStatusService } from './health-status.service';

describe('HealthStatusService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [HealthStatusService]
    });
  });

  it('should be created', inject([HealthStatusService], (service: HealthStatusService) => {
    expect(service).toBeTruthy();
  }));
});
