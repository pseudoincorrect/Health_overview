import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { SettingsRoutingModule } from '@app/pages/settings/settings-routing.module';
import { SettingsComponent } from '@app/pages/settings/settings.component';
import { MaterialModule } from '@app/material.module';
import { SharedModule } from '@app/shared';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { AngularSvgIconModule } from 'angular-svg-icon';

@NgModule({
    imports: [
        CommonModule,
        SharedModule,
        FlexLayoutModule,
        MaterialModule,
        SettingsRoutingModule,
        AngularSvgIconModule
    ],
    declarations: [
        SettingsComponent
    ],
    providers: [ EmployeesService ]
})
export class SettingsModule {  }
