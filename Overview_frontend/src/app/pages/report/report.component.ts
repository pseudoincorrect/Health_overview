import {  Component, ElementRef, OnInit, ViewChild } from '@angular/core';
import { Chart } from 'chart.js';
import { EmployeesService } from '@app/services/localServices/employees.service';
import * as moment from 'moment';

@Component({
    selector: 'app-report',
    templateUrl: './report.component.html',
    styleUrls: ['./report.component.scss']
})
export class ReportComponent implements OnInit {
    data: any;
    options: any;
    reportChart: Chart;
    date: string;

    @ViewChild('detailCanvas') reportCanvas: ElementRef;
    
    constructor(
        private employeesService: EmployeesService
    ) {}
    
    ngOnInit() {
        this.date = moment().format('MMMM Do YYYY').toString();
        this.createChartData();
        this.createCharts();
    }
    
    createChartData() {
        this.data = {
            labels: ['good', 'need rest', 'warning'],
            datasets: [{
                data: [67, 12, 5], 
                backgroundColor: [
                    'green',
                    'yellow',
                    'red'
                ],
                label: 'dataset 1'
            }]
        };
        
        this.options = {
            responsive: true,
            legend: 
            {
                position: 'top',
            },
            title: 
            {
                display: true,
                text: 'Overall health status'
            },
            animation: 
            {
                animateScale: true,
                animateRotate: true
            }
        };
    }
    
    createCharts () {
        let ctx = this.reportCanvas.nativeElement.getContext('2d'); 
        this.reportChart = new Chart (
            ctx,
            {
                type : 'doughnut',
                data : this.data,
                options : this.options
            } 
        );
    }
}
