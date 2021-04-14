import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { Route } from '@app/core';
import { DetailsComponent } from '@app/pages/details/details.component';

const routes: Routes = [
  { path: '', component: DetailsComponent, data: { title: 'Details' } }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
  providers: []
})
export class DetailsRoutingModule { }
