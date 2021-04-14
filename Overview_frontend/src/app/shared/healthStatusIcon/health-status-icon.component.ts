import { Component, OnInit, Input } from '@angular/core';
import { HealthStatusService } from '@app/services/localServices/health-status.service';

@Component({
  selector: 'app-health-status-icon',
  templateUrl: './health-status-icon.component.html',
  styleUrls: ['./health-status-icon.component.scss']
})
export class HealthStatusIconComponent implements OnInit {
  public iconColor: string;

  @Input()
  empId: string;

  constructor(
    private healthStatusService: HealthStatusService
  ) { }

  ngOnInit() {
    console.log(this.empId);
  }

  changeColor(): any {
    let healthLvl: number = this.healthStatusService.getHealthStatus(this.empId);
    let color: string;
    switch (healthLvl) {
      case 1:
        color = "#01FE3C"
      break;
      case 2:
        color = "#BEFE28"
      break;
      case 3:
        color = "#FECC28"
      break;
      case 4:
        color = "#FE8C28"
        break;
      case 5:
        color = "#FE2828"
        break;
      default:
        color = "grey"
        break;
    }
    return {'fill':color} ;
  }

}
