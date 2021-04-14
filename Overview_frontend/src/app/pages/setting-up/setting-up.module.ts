import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { SettingUpRoutingModule } from '@app/pages/setting-up/setting-up-routing.module';
import { SettingUpComponent } from '@app/pages/setting-up/setting-up.component';
import { MaterialModule } from '@app/material.module';
import { SharedModule } from '@app/shared';
import { AngularSvgIconModule } from 'angular-svg-icon';
import { AuthenticationHttpService } from '@app/services/httpServices/authentication-http.service';
import { AuthenticationService } from '@app/core/authentication/authentication.service';

@NgModule({
    imports: [
        CommonModule,
        SharedModule,
        FlexLayoutModule,
        MaterialModule,
        SettingUpRoutingModule,
        AngularSvgIconModule
    ],
    declarations: [
        SettingUpComponent
    ],
    providers: [ 
        AuthenticationService,
        AuthenticationHttpService
     ]
})
export class SettingUpModule {  }
