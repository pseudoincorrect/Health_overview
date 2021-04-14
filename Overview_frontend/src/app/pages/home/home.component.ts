import { Component, OnInit } from '@angular/core';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { DialogEditNameService } from '@app/services/localServices/dialog-edit-name.service';
import { HealthStatusService } from '@app/services/localServices/health-status.service';

@Component({
    selector: 'app-home',
    templateUrl: './home.component.html',
    styleUrls: ['./home.component.scss']
})
export class HomeComponent implements OnInit {

    constructor (
        public employeesService: EmployeesService,
        public editNameService: DialogEditNameService,
        public healthStatusService: HealthStatusService
    ) { }
    
    ngOnInit() { }

    openDialogEditName(nodeAddress: string): void {
        this.editNameService.openDialog(nodeAddress);
    }  
}
