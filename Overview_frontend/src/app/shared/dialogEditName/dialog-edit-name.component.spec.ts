import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { DialogEditNameComponent } from './dialog-edit-name.component';

describe('DialogEditNameComponent', () => {
  let component: DialogEditNameComponent;
  let fixture: ComponentFixture<DialogEditNameComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ DialogEditNameComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DialogEditNameComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
