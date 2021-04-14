import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { ReportRoutingModule } from '@app/pages/report/report-routing.module';
import { ReportComponent } from '@app/pages/report/report.component';
import { MaterialModule } from '@app/material.module';
import { SharedModule } from '@app/shared';
import { AngularSvgIconModule } from 'angular-svg-icon';

@NgModule({
    imports: [
        CommonModule,
        SharedModule,
        FlexLayoutModule,
        MaterialModule,
        ReportRoutingModule,
        AngularSvgIconModule
    ],
    declarations: [
        ReportComponent
    ]
})
export class ReportModule {  }
