import { Injectable } from "@angular/core";
import { NotificationsHttpService } from '@app/services/httpServices/notifications-http.service';
import { interval } from "rxjs";

@Injectable({
    providedIn: "root"
})
export class NotificationsService {
    constructor(
        private notificationsHttpService: NotificationsHttpService
    ) {

    }

    public async checkNotif() {
        let notifs: any; 
        notifs = await this.notificationsHttpService.getNotifications();
        console.log("notifs: ", notifs);
    }
}
