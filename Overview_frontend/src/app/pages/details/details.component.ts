import {
    Component,
    ElementRef,
    OnInit,
    ViewChild } from '@angular/core';
import { Chart } from 'chart.js';
import { ActivatedRoute } from '@angular/router';
import { Employee, EmployeesService, DataChart } from '@app/services/localServices/employees.service';
import { DialogEditNameService } from '@app/services/localServices/dialog-edit-name.service';
import { NotificationsHttpService } from '@app/services/httpServices/notifications-http.service';
import { HealthStatusService } from '@app/services/localServices/health-status.service';

@Component({
    selector: 'app-details',
    templateUrl: './details.component.html',
    styleUrls: ['./details.component.scss']
})

export class DetailsComponent implements OnInit {
    id: string;
    dataTemperature: any;
    dataHumidity: any;
    dataHeartRate: any;
    options: any;
    detailsChartTemperature: Chart;
    detailsChartHumidity: Chart;
    detailsChartHeartRate: Chart;
    employee: Employee;

    @ViewChild('detailCanvasTemperature') detailCanvasTemperature: ElementRef;
    @ViewChild('detailCanvasHumidity') detailCanvasHumidity: ElementRef;
    @ViewChild('detailCanvasHeartRate') detailCanvasHeartRate: ElementRef;

    constructor(
        private route: ActivatedRoute,
        private employeesService: EmployeesService,
        private notificationService: NotificationsHttpService,
        private editNameService: DialogEditNameService
    ) {}

    ngOnInit() {
        if (!this.employeesService.employees) {
            this.employeesService.init().then(( () => {
                this.id = this.route.snapshot.paramMap.get('id');
                this.employee = this.employeesService.employees.findEmployee(this.id);
                this.createChart();
             } ).bind(this));
        }
        else {
            this.id = this.route.snapshot.paramMap.get('id');
            this.employee = this.employeesService.employees.findEmployee(this.id);
            this.createChart();
        }
        setInterval(this.updateCharts.bind(this), 5000);
    }

    notify(nodeAddress: string): void {
        this.notificationService.postNotification(nodeAddress);
    }

    openDialogEditName(): void {
        let employee = this.employeesService.employees.findEmployee(this.id);
        this.editNameService.openDialog(employee.nodeAddress);
    }

    createChart() {
        let dataChart: DataChart;
        dataChart = this.employeesService.employees.getDetailsFrom(
            this.id,
            Number(Date.now() - 24 * 3600 * 1000)
        );

        this.dataTemperature = {
            labels: dataChart.timeStampFormatted,
            datasets:
            [
                {
                    data: dataChart.temperature,
                    radius: 1,
                    label: 'Temperature' ,
                    borderColor: 'rgba(39, 252, 46, 1',
                    backgroundColor: 'rgba(39, 252, 46, 0.05)',
                    fill: 'start',
                }
            ]
        };

        this.dataHumidity = {
            labels: dataChart.timeStampFormatted,
            datasets:
            [
                {
                    data: dataChart.humidity,
                    radius: 1,
                    label: 'Humidity' ,
                    borderColor: 'rgba(39, 43, 252, 1',
                    backgroundColor: 'rgba(39, 43, 252, 0.2)',
                    fill: 'start',
                }
            ]
        };

        this.dataHeartRate = {
            labels: dataChart.timeStampFormatted,
            datasets:
            [ {
                data: dataChart.heartRate,
                radius: 1,
                borderColor: 'rgba(252, 39, 39, 1)',
                backgroundColor: 'rgba(252, 39, 39, 0.2)',
                label: 'Heart Rate',
                fill: 'start',
            } ]
        };

        this.options = {
            responsive: true,
            maintainAspectRatio: true,
            scales: {
                xAxes: [ {
                    gridLines: { display: false},
                    ticks: {
                        maxTicksLimit: 3,
                        maxRotation: 0,
                        minRotation: 0
                    }
                } ],

                yAxes: [ {
                    gridLines: { display: false},
                    ticks: {
                        maxTicksLimit: 3,
                    },
                } ],
            },
            legend: {
                display: false
             },
             tooltips: {
                mode: 'nearest',
                intersect: false
            },
            hover: {
                mode: 'nearest',
                intersect: false
            },

        };

        let ctx = this.detailCanvasTemperature.nativeElement.getContext('2d');
        this.detailsChartTemperature = new Chart (
            ctx,
            {
                type : 'line',
                data : this.dataTemperature,
                options : this.options
            }
        );

        ctx = this.detailCanvasHumidity.nativeElement.getContext('2d');
        this.detailsChartHumidity = new Chart (
            ctx,
            {
                type : 'line',
                data : this.dataHumidity,
                options : this.options
            }
        );

        ctx = this.detailCanvasHeartRate.nativeElement.getContext('2d');
        this.detailsChartHeartRate = new Chart (
            ctx,
            {
                type : 'line',
                data : this.dataHeartRate,
                options : this.options
            }
        );
    }

    updateCharts() {
        let dataChart: DataChart;
        dataChart = this.employeesService.employees.getDetailsFrom(
            this.id,
            Number(Date.now() - 24 * 3600 * 1000)
        );

        this.updateData(this.detailsChartTemperature,
                    dataChart.timeStampFormatted, dataChart.temperature);
        this.updateData(this.detailsChartHumidity,
                    dataChart.timeStampFormatted, dataChart.humidity);
        this.updateData(this.detailsChartHeartRate,
                    dataChart.timeStampFormatted, dataChart.heartRate);
    }

    updateData(chart: Chart, label: string[], data: number[]) {
        chart.data.labels = label;
        chart.data.datasets.forEach((dataset) => {
            dataset.data = data;
        });
        chart.update({
            duration: 500,
            easing: 'easeOutBack'
        });
    }
}
