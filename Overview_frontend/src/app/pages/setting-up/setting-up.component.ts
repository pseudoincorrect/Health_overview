import { Component, OnInit } from '@angular/core';
import { AuthenticationHttpService } from '@app/services/httpServices/authentication-http.service';
import { AuthenticationService } from '@app/core/authentication/authentication.service';
import {saveAs} from 'file-saver';

@Component({
  selector: 'app-setting-up',
  templateUrl: './setting-up.component.html',
  styleUrls: ['./setting-up.component.scss']
})
export class SettingUpComponent implements OnInit {

  constructor(  
    public authService: AuthenticationService,
    private authHttpService: AuthenticationHttpService ) { }

  ngOnInit() {
  }

  async downloadCredentials() {
    let token;
    token = await this.authHttpService.getGatewayCredentials();
    console.log(token);

    let blob = new Blob([token], {type: 'text/plain;charset=utf-8'});
    saveAs(blob, 'credentialOverview.cred');
  }
}
