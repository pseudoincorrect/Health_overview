import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { TranslateModule } from '@ngx-translate/core';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { AuthenticationService } from '@app/core/authentication/authentication.service';
import { MockAuthenticationService } from '@app/core/authentication/authentication.service.mock';
import { CoreModule } from '@app/core/core.module';
import { ShellComponent } from '@app/core/shell/shell.component';

describe('ShellComponent', () => {
  let component: ShellComponent;
  let fixture: ComponentFixture<ShellComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule,
        TranslateModule.forRoot(),
        BrowserAnimationsModule,
        CoreModule
      ],
      providers: [
        { provide: AuthenticationService, useClass: MockAuthenticationService }
      ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ShellComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
