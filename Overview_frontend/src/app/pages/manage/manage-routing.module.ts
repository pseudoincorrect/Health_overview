import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ManageComponent } from '@app/pages/manage/manage.component';

const routes: Routes = [
  { path: '', component: ManageComponent, data: { title: 'Manage' } }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
  providers: []
})
export class ManageRoutingModule { }
