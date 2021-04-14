import { NgModule } from '@angular/core';
import { Routes, RouterModule, PreloadAllModules } from '@angular/router';
import { Route } from '@app/core';

const routes: Routes = [
  Route.withShell([
    { path: 'home', loadChildren: 'app/pages/home/home.module#HomeModule' },
    { path: 'about', loadChildren: 'app/pages/about/about.module#AboutModule' },
    { path: 'report', loadChildren: 'app/pages/report/report.module#ReportModule' },
    // { path: 'manage', loadChildren: 'app/pages/manage/manage.module#ManageModule' },
    { path: 'details/:id', loadChildren: 'app/pages/details/details.module#DetailsModule' },
    { path: 'settings', loadChildren: 'app/pages/settings/settings.module#SettingsModule' },
    { path: 'setting-up', loadChildren: 'app/pages/setting-up/setting-up.module#SettingUpModule' },
    { path: '', loadChildren: 'app/pages/home/home.module#HomeModule' }
  ]),
  { path: '**', redirectTo: '/home', pathMatch: 'full' }
];


@NgModule({
  imports: [RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules })],
  exports: [RouterModule],
  providers: []
})
export class AppRoutingModule { }
