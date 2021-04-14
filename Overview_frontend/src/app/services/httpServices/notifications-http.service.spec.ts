import { TestBed, inject } from '@angular/core/testing';

import { NotificationsHttpService } from './notifications-http.service';

describe('NotificationsHttpService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [NotificationsHttpService]
    });
  });

  it('should be created', inject([NotificationsHttpService], (service: NotificationsHttpService) => {
    expect(service).toBeTruthy();
  }));
});
