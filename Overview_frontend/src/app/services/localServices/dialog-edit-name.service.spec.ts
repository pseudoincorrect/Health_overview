import { TestBed, inject } from '@angular/core/testing';

import { DialogEditNameService } from './dialog-edit-name.service';

describe('DialogEditNameService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [DialogEditNameService]
    });
  });

  it('should be created', inject([DialogEditNameService], (service: DialogEditNameService) => {
    expect(service).toBeTruthy();
  }));
});
