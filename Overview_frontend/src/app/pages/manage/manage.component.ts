import { Component, OnInit } from '@angular/core';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { DialogEditNameService } from '@app/services/localServices/dialog-edit-name.service';

@Component({
    selector: 'app-manage',
    templateUrl: './manage.component.html',
    styleUrls: ['./manage.component.scss']
})
export class ManageComponent implements OnInit {
    
    constructor (
        public employeesService: EmployeesService,
        public editNameService: DialogEditNameService) { }
    
    ngOnInit() { }

    openDialogEditName(nodeAddress: string): void {
        this.editNameService.openDialog(nodeAddress);
    }  
}
