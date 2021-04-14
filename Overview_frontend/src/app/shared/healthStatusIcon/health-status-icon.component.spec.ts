import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { HealthStatusIconComponent } from './health-status-icon.component';

describe('HealthStatusIconComponent', () => {
  let component: HealthStatusIconComponent;
  let fixture: ComponentFixture<HealthStatusIconComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ HealthStatusIconComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(HealthStatusIconComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
