import { Component, OnInit } from '@angular/core';
import { Router, NavigationEnd, ActivatedRoute } from '@angular/router';
import { Title } from '@angular/platform-browser';
import { merge } from 'rxjs';
import { filter, map, mergeMap } from 'rxjs/operators';

import { environment } from '@env/environment';
import { Logger } from '@app/core';

const log = new Logger('App');

@Component({
    selector: 'app-root',
    templateUrl: './app.component.html',
    styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
    
    constructor(private router: Router,
        private activatedRoute: ActivatedRoute,
        private titleService: Title) { 
    }
        
    ngOnInit() {
        // Setup logger
        if (environment.production) {
            Logger.enableProductionMode();
        }
        
        const onNavigationEnd = this.router.events.pipe(filter(event => event instanceof NavigationEnd));
        
        // Change page title on navigation or language change, based on route data
        merge(onNavigationEnd)
            .pipe(
                map(() => {
                    let route = this.activatedRoute;
                    while (route.firstChild) {
                        route = route.firstChild;
                    }
                    return route;
                }),
                filter(route => route.outlet === 'primary'),
                mergeMap(route => route.data)
                )
            .subscribe(event => {
                const title = event['title'];
                if (title) {
                    this.titleService.setTitle(title);
                }
            });
        }

}
