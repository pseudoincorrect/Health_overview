import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { DialogEditNameComponent } from './dialog-edit-name.component';
import { MatFormFieldModule, MatDialogModule, MatInputModule } from '@angular/material';
import { MaterialModule } from '@app/material.module';

@NgModule({
    imports: [
    CommonModule,
        MaterialModule,
        FormsModule,
        ReactiveFormsModule,
        MatFormFieldModule,
        MatDialogModule,
        MatInputModule
    ],
    declarations: [
        DialogEditNameComponent
    ],
    entryComponents: [DialogEditNameComponent]
})
export class DialogEditNameModule {  }
