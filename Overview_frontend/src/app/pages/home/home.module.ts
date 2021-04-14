import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FlexLayoutModule } from '@angular/flex-layout';
import { HomeRoutingModule } from '@app/pages/home/home-routing.module';
import { HomeComponent } from '@app/pages/home/home.component';
import { MaterialModule } from '@app/material.module';
import { SharedModule } from '@app/shared';
import { AngularSvgIconModule } from 'angular-svg-icon';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { OrderModule } from 'ngx-order-pipe';

@NgModule({
    imports: [
        CommonModule,
        SharedModule,
        FlexLayoutModule,
        MaterialModule,
        HomeRoutingModule,
        AngularSvgIconModule,
        FormsModule,
        ReactiveFormsModule,
        OrderModule, 
    ],
    declarations: [
        HomeComponent
    ],

})
export class HomeModule {  }
