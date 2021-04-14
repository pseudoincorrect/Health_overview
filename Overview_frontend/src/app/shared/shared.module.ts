import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FlexLayoutModule } from '@angular/flex-layout';

import { MaterialModule } from '@app/material.module';
import { LoaderComponent } from '@app/shared/loader/loader.component';
import { HealthStatusIconComponent } from '@app/shared/healthStatusIcon/health-status-icon.component';
import { AngularSvgIconModule } from 'angular-svg-icon';

@NgModule({
  imports: [
    FlexLayoutModule,
    MaterialModule,
    CommonModule,
    AngularSvgIconModule
  ],
  declarations: [
    LoaderComponent,
    HealthStatusIconComponent,
  ],
  exports: [
    LoaderComponent,
    HealthStatusIconComponent
  ]
})
export class SharedModule { }
