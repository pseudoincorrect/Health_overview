import { Component, OnInit, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material';

export interface DialogData {
  name: string;
}

@Component({
  selector: 'app-dialog-edit-name',
  templateUrl: './dialog-edit-name.component.html',
  styleUrls: ['./dialog-edit-name.component.scss']
})
export class DialogEditNameComponent implements OnInit {

  constructor(
      public dialogRef: MatDialogRef<DialogEditNameComponent>,
      @Inject(MAT_DIALOG_DATA) public data: DialogData) {}
      
  onNoClick(): void {
      this.dialogRef.close();
  }
  ngOnInit() {
  }

}
