import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { MaterialModule } from '@app/material.module';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { SharedModule } from '@app/shared/shared.module';
import { AngularSvgIconModule } from 'angular-svg-icon';
import { ManageRoutingModule } from './manage-routing.module';
import { ManageComponent } from './manage.component';

@NgModule({
    imports: [
        CommonModule,
        SharedModule,
        FlexLayoutModule,
        MaterialModule,
        ManageRoutingModule,
        AngularSvgIconModule
    ],
    declarations: [
        ManageComponent
    ]
})
export class ManageModule { }
