import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { finalize } from 'rxjs/operators';

import { environment } from '@env/environment';
import { Logger, AuthenticationService } from '@app/core';
import { EmployeesService } from '@app/services/localServices/employees.service';

const log = new Logger('Login');

@Component({
    selector: 'app-login',
    templateUrl: './login.component.html',
    styleUrls: ['./login.component.scss']
})
export class LoginComponent implements OnInit {
    
    version: string = environment.version;
    error: string;
    loginForm: FormGroup;
    
    constructor(
        private router: Router, 
        private formBuilder: FormBuilder,
        private authenticationService: AuthenticationService,
        public  employeeService: EmployeesService) {
        
        this.createForm();
    }
    
    ngOnInit() { }
    
    login() {
        
        this.authenticationService.login(this.loginForm.value);
        
        this.authenticationService.login(this.loginForm.value)
        .pipe(finalize(() => {
            this.loginForm.markAsPristine();
        }))
        .subscribe(credentials => {
            log.debug(`${credentials.username} successfully logged in`);
            this.employeeService.refresh();
            this.router.navigate(['/home'], { replaceUrl: true });
        }, error => {
            log.debug(`Login error: ${error}`);
            this.error = error;
        });
    }
    
    accessDemonstration() {
        this.authenticationService.loginDemo()
        .subscribe(credentials => {
            log.debug(`${credentials.username} successfully logged in`);
            this.employeeService.refresh();
            this.router.navigate(['/home'], { replaceUrl: true });
        }, error => {
            log.debug(`Login error: ${error}`);
            this.error = error;
        });
    } 
    
    private createForm() {
        this.loginForm = this.formBuilder.group({
            username: ['', Validators.required],
            password: ['', Validators.required],
            remember: true
        });
    }
}
