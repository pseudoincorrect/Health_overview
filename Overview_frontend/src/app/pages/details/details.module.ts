import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { TranslateModule } from '@ngx-translate/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { AngularSvgIconModule } from 'angular-svg-icon';
import { MaterialModule } from '@app/material.module';
import { DetailsRoutingModule } from '@app/pages/details/details-routing.module';
import { DetailsComponent } from '@app/pages/details/details.component';
import { SharedModule } from '@app/shared';

@NgModule({
  imports: [
    CommonModule,
    TranslateModule,
    FlexLayoutModule,
    MaterialModule,
    AngularSvgIconModule,
    DetailsRoutingModule,
    SharedModule,
  ],
  declarations: [
    DetailsComponent
  ]
})
export class DetailsModule { }
