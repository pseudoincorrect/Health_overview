
const defaultNotif = {
    nodeAddress: "",
    notifMessage:  ""
}


class NotifMocker {
    constructor () {
        this.address = 0;
    }

    getOneMock () {
        let mock_Notif = JSON.parse(JSON.stringify(defaultNotif));

        mock_Notif.nodeAddress = "address_" + this.pad(this.address, 3);
        this.address = this.address + 1;
        if (this.address > 5)
            this.address = 0;

        mock_Notif.notifMessage   = "N_G_NOTIF";
        return mock_Notif;
    }

    pad(num, size) {
        var s = "000000000" + num;
        return s.substr(s.length-size);
    }

}


exports.NotifMocker = NotifMocker; 
