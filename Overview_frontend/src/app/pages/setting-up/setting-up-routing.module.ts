import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { SettingUpComponent } from '@app/pages/setting-up/setting-up.component';

const routes: Routes = [
  { path: '', component: SettingUpComponent, data: { title: 'Setting-up' } }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
  providers: []
})
export class SettingUpRoutingModule { }
