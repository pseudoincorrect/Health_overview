import { Title } from '@angular/platform-browser';
import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { ObservableMedia } from '@angular/flex-layout';

import { AuthenticationService } from '@app/core/authentication/authentication.service';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { NotificationsService } from '../../services/localServices/notifications.service';

@Component({
  selector: 'app-shell',
  templateUrl: './shell.component.html',
  styleUrls: ['./shell.component.scss']
})
export class ShellComponent implements OnInit {
  isLoading: boolean;

  constructor(private router: Router,
              private titleService: Title,
              private media: ObservableMedia,
              private authenticationService: AuthenticationService,
              public employeeService: EmployeesService,
              public notificationsService: NotificationsService
              ) { }

  ngOnInit() {
        this.refresh();
        setInterval(this.refresh.bind(this), 5000);

  }
          
  refresh(): void {
    this.employeeService.refresh();
    this.notificationsService.checkNotif();
  }

  logout() {
    this.authenticationService.logout()
      .subscribe(() => this.router.navigate(['/login'], { replaceUrl: true }));
  }

  get username(): string | null {
    const credentials = this.authenticationService.credentials;
    return credentials ? credentials.username : null;
  }

  get isMobile(): boolean {
    return this.media.isActive('xs') || this.media.isActive('sm');
  }

  get title(): string {
    return this.titleService.getTitle();
  }
}
