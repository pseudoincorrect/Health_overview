import {Injectable} from '@angular/core';
import { Subject } from 'rxjs';

import { MatDialog } from '@angular/material';
import { EmployeesService } from '@app/services/localServices/employees.service';
import { DialogEditNameComponent } from '@app/shared/dialogEditName/dialog-edit-name.component';

@Injectable({providedIn: 'root'})
export class DialogEditNameService {
    isShowing = false;
    modal = new Subject();
    name: string;

    constructor (
        public employeeService: EmployeesService,
        public dialog: MatDialog ) {}

    public openDialog(nodeAddress: string): void {
        const dialogRef = this.dialog.open(DialogEditNameComponent, {
            width: '250px',
            data: {name: this.name}
        });
        
        dialogRef.afterClosed().subscribe(result => {
            if (result) {
                this.name = result;
                this.employeeService.updateEmployeeName(nodeAddress, this.name);
            }
        });
    }  
}
