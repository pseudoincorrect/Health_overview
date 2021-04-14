import { HTTP_INTERCEPTORS } from '@angular/common/http';
import { NgModule, ErrorHandler } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { ServiceWorkerModule } from '@angular/service-worker';
import { AppRoutingModule } from '@app/app-routing.module';
import { AppComponent } from '@app/app.component';
import { CoreModule, AuthenticationService } from '@app/core';
import { TokenInterceptor } from '@app/core/http/token.interceptor';
import { LoginModule } from '@app/pages/login/login.module';
import { MaterialModule } from '@app/material.module';
import { SharedModule } from '@app/shared';
import { AppErrorHandler } from '@app/core/errors/appErrorHandler';
import { environment } from '@env/environment';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { AuthenticationHttpService } from '@app/services/httpServices/authentication-http.service';
import { DialogEditNameService } from '@app/services/localServices/dialog-edit-name.service';
import { DialogEditNameModule } from '@app/shared/dialogEditName/dialog-edit-name.module';
import { NotificationsHttpService } from '@app/services/httpServices/notifications-http.service';

DialogEditNameModule
@NgModule({
    imports: [
    BrowserModule,
        ServiceWorkerModule.register('./ngsw-worker.js', { enabled: environment.production }),
        FormsModule,
        ReactiveFormsModule,
        HttpModule,
        BrowserAnimationsModule,
        MaterialModule,
        CoreModule,
        SharedModule,
        LoginModule,
        DialogEditNameModule,
        AppRoutingModule
    ],
    declarations: [
        AppComponent
    ],
    providers: [
        EmployeesService,
        NotificationsHttpService,
        AuthenticationHttpService,
        DialogEditNameService,
        AuthenticationService,
        {
            provide: HTTP_INTERCEPTORS,
            useClass: TokenInterceptor,
            multi: true
        },
        {provide: ErrorHandler, useClass: AppErrorHandler}
    ],
    bootstrap: [AppComponent]
})
export class AppModule { }
