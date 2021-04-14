import { ErrorHandler } from '@angular/core';


export class AppErrorHandler implements ErrorHandler {
    handleError(error: any) {
        console.log('An unexpected error happened');
        console.log(error);
      // do something with the exception
    }
  }
