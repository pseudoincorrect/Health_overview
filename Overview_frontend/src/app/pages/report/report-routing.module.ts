import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ReportComponent } from '@app/pages/report/report.component';

const routes: Routes = [
  { path: '', component: ReportComponent, data: { title: 'Daily Report' } }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
  providers: []
})
export class ReportRoutingModule { }
