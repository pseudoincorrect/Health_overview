(window.webpackJsonp=window.webpackJsonp||[]).push([[3],{coki:function(t,e,l){"use strict";l.d(e,"a",function(){return h}),l.d(e,"d",function(){return b}),l.d(e,"b",function(){return g}),l.d(e,"c",function(){return m});var n=l("mrSG"),a=l("CcnG"),i=(l("t/Na"),l("F/XL")),o=l("XlPw"),r=l("67Y/"),s=l("xMyE"),u=l("9Z1F"),d=l("2WpN"),c=l("S1nX"),b=function(){function t(t){this.http=t,this.iconsByUrl=new Map,this.iconsLoadingByUrl=new Map}return t.prototype.addSvg=function(t,e){if(!this.iconsByUrl.has(t)){var l=document.createElement("DIV");l.innerHTML=e;var n=l.querySelector("svg");this.iconsByUrl.set(t,n)}},t.prototype.loadSvg=function(t){var e=this;if(this.iconsByUrl.has(t))return Object(i.a)(this.iconsByUrl.get(t));if(this.iconsLoadingByUrl.has(t))return this.iconsLoadingByUrl.get(t);var l=this.http.get(t,{responseType:"text"}).pipe(Object(r.a)(function(t){var e=document.createElement("DIV");return e.innerHTML=t,e.querySelector("svg")}),Object(s.a)(function(l){return e.iconsByUrl.set(t,l)}),Object(u.a)(function(t){return console.error(t),Object(o.a)(t)}),Object(d.a)(function(){return e.iconsLoadingByUrl.delete(t)}),Object(c.a)());return this.iconsLoadingByUrl.set(t,l),l},t.prototype.unloadSvg=function(t){this.iconsByUrl.has(t)&&this.iconsByUrl.delete(t)},t}();function g(t,e){return t||new b(e)}new a.A,new a.K;var m=function(){function t(t,e,l,n){this.element=t,this.differs=e,this.renderer=l,this.iconReg=n,this.stretch=!1}return Object.defineProperty(t.prototype,"svgStyle",{set:function(t){this._svgStyle=t,!this.differ&&t&&(this.differ=this.differs.find(t).create())},enumerable:!0,configurable:!0}),t.prototype.ngOnInit=function(){this.init()},t.prototype.ngOnDestroy=function(){this.destroy()},t.prototype.ngOnChanges=function(t){t.src&&(this.svg&&this.destroy(),this.init()),t.stretch&&this.stylize()},t.prototype.ngDoCheck=function(){if(this.svg&&this.differ){var t=this.differ.diff(this._svgStyle);t&&this.applyChanges(t)}},t.prototype.init=function(){var t=this;this.icnSub=this.iconReg.loadSvg(this.src).subscribe(function(e){t.setSvg(e),t.resetDiffer()})},t.prototype.destroy=function(){this.svg=void 0,this.differ=void 0,this.icnSub&&this.icnSub.unsubscribe()},t.prototype.resetDiffer=function(){this._svgStyle&&!this.differ&&(this.differ=this.differs.find(this._svgStyle).create())},t.prototype.setSvg=function(t){if(t){this.svg=t;var e=t.cloneNode(!0),l=this.element.nativeElement;l.innerHTML="",this.renderer.appendChild(l,e),this.stylize()}},t.prototype.stylize=function(){if(this.svg){var t=this.element.nativeElement.firstChild;!0===this.stretch?this.renderer.setAttribute(t,"preserveAspectRatio","none"):!1===this.stretch&&this.renderer.removeAttribute(t,"preserveAspectRatio")}},t.prototype.applyChanges=function(t){var e=this;t.forEachRemovedItem(function(t){return e.setStyle(t.key,null)}),t.forEachAddedItem(function(t){return e.setStyle(t.key,t.currentValue)}),t.forEachChangedItem(function(t){return e.setStyle(t.key,t.currentValue)})},t.prototype.setStyle=function(t,e){var l=Object(n.c)(t.split("."),2),a=l[0],i=l[1],o=this.element.nativeElement.firstChild;null!==(e=null!==e&&i?""+e+i:e)?this.renderer.setStyle(o,a,e):this.renderer.removeStyle(o,a)},t}(),h=function(){}},zDS7:function(t,e,l){"use strict";l.r(e);var n=l("CcnG"),a=function(){},i=l("t68o"),o=l("zbXB"),r=l("NcP4"),s=l("xYTU"),u=l("pMnS"),d=l("21Lb"),c=l("OzfB"),b=l("lzlj"),g=l("FVSy"),m=l("kWGw"),h=l("M2Lx"),p=l("ZYjt"),f=l("Wf4p"),Z=l("Fzqc"),y=l("dWZg"),v=l("wFw1"),k=l("gIcY"),x=l("lLAP"),w=n.Pa({encapsulation:2,styles:[".mat-slide-toggle{display:inline-block;height:24px;max-width:100%;line-height:24px;white-space:nowrap;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;outline:0}.mat-slide-toggle.mat-checked .mat-slide-toggle-thumb-container{transform:translate3d(16px,0,0)}[dir=rtl] .mat-slide-toggle.mat-checked .mat-slide-toggle-thumb-container{transform:translate3d(-16px,0,0)}.mat-slide-toggle.mat-disabled .mat-slide-toggle-label,.mat-slide-toggle.mat-disabled .mat-slide-toggle-thumb-container{cursor:default}.mat-slide-toggle-label{display:flex;flex:1;flex-direction:row;align-items:center;height:inherit;cursor:pointer}.mat-slide-toggle-content{white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.mat-slide-toggle-label-before .mat-slide-toggle-label{order:1}.mat-slide-toggle-label-before .mat-slide-toggle-bar{order:2}.mat-slide-toggle-bar,[dir=rtl] .mat-slide-toggle-label-before .mat-slide-toggle-bar{margin-right:8px;margin-left:0}.mat-slide-toggle-label-before .mat-slide-toggle-bar,[dir=rtl] .mat-slide-toggle-bar{margin-left:8px;margin-right:0}.mat-slide-toggle-bar-no-side-margin{margin-left:0;margin-right:0}.mat-slide-toggle-thumb-container{position:absolute;z-index:1;width:20px;height:20px;top:-3px;left:0;transform:translate3d(0,0,0);transition:all 80ms linear;transition-property:transform;cursor:-webkit-grab;cursor:grab}.mat-slide-toggle-thumb-container.mat-dragging,.mat-slide-toggle-thumb-container:active{cursor:-webkit-grabbing;cursor:grabbing;transition-duration:0s}._mat-animation-noopable .mat-slide-toggle-thumb-container{transition:none}[dir=rtl] .mat-slide-toggle-thumb-container{left:auto;right:0}.mat-slide-toggle-thumb{height:20px;width:20px;border-radius:50%;box-shadow:0 2px 1px -1px rgba(0,0,0,.2),0 1px 1px 0 rgba(0,0,0,.14),0 1px 3px 0 rgba(0,0,0,.12)}.mat-slide-toggle-bar{position:relative;width:36px;height:14px;flex-shrink:0;border-radius:8px}.mat-slide-toggle-input{bottom:0;left:10px}[dir=rtl] .mat-slide-toggle-input{left:auto;right:10px}.mat-slide-toggle-bar,.mat-slide-toggle-thumb{transition:all 80ms linear;transition-property:background-color;transition-delay:50ms}._mat-animation-noopable .mat-slide-toggle-bar,._mat-animation-noopable .mat-slide-toggle-thumb{transition:none}.mat-slide-toggle-ripple{position:absolute;top:calc(50% - 23px);left:calc(50% - 23px);height:46px;width:46px;z-index:1;pointer-events:none}@media screen and (-ms-high-contrast:active){.mat-slide-toggle-thumb{background:#fff;border:1px solid #000}.mat-slide-toggle.mat-checked .mat-slide-toggle-thumb{background:#000;border:1px solid #fff}.mat-slide-toggle-bar{background:#fff}}@media screen and (-ms-high-contrast:black-on-white){.mat-slide-toggle-bar{border:1px solid #000}}"],data:{}});function S(t){return n.kb(2,[n.gb(402653184,1,{_thumbEl:0}),n.gb(402653184,2,{_thumbBarEl:0}),n.gb(402653184,3,{_inputElement:0}),n.gb(402653184,4,{_ripple:0}),(t()(),n.Ra(4,0,[["label",1]],null,10,"label",[["class","mat-slide-toggle-label"]],null,null,null,null,null)),(t()(),n.Ra(5,0,[[2,0],["toggleBar",1]],null,6,"div",[["class","mat-slide-toggle-bar"]],[[2,"mat-slide-toggle-bar-no-side-margin",null]],null,null,null,null)),(t()(),n.Ra(6,0,[[3,0],["input",1]],null,0,"input",[["class","mat-slide-toggle-input cdk-visually-hidden"],["type","checkbox"]],[[8,"id",0],[8,"required",0],[8,"tabIndex",0],[8,"checked",0],[8,"disabled",0],[1,"name",0],[1,"aria-label",0],[1,"aria-labelledby",0]],[[null,"change"],[null,"click"]],function(t,e,l){var n=!0,a=t.component;return"change"===e&&(n=!1!==a._onChangeEvent(l)&&n),"click"===e&&(n=!1!==a._onInputClick(l)&&n),n},null,null)),(t()(),n.Ra(7,0,[[1,0],["thumbContainer",1]],null,4,"div",[["class","mat-slide-toggle-thumb-container"]],null,[[null,"slidestart"],[null,"slide"],[null,"slideend"]],function(t,e,l){var n=!0,a=t.component;return"slidestart"===e&&(n=!1!==a._onDragStart()&&n),"slide"===e&&(n=!1!==a._onDrag(l)&&n),"slideend"===e&&(n=!1!==a._onDragEnd()&&n),n},null,null)),(t()(),n.Ra(8,0,null,null,0,"div",[["class","mat-slide-toggle-thumb"]],null,null,null,null,null)),(t()(),n.Ra(9,0,null,null,2,"div",[["class","mat-slide-toggle-ripple mat-ripple"],["mat-ripple",""]],[[2,"mat-ripple-unbounded",null]],null,null,null,null)),n.Qa(10,212992,[[4,4]],0,f.x,[n.k,n.z,y.a,[2,f.m],[2,v.a]],{centered:[0,"centered"],radius:[1,"radius"],animation:[2,"animation"],disabled:[3,"disabled"],trigger:[4,"trigger"]},null),n.eb(11,{enterDuration:0}),(t()(),n.Ra(12,0,[["labelContent",1]],null,2,"span",[["class","mat-slide-toggle-content"]],null,[[null,"cdkObserveContent"]],function(t,e,l){var n=!0;return"cdkObserveContent"===e&&(n=!1!==t.component._onLabelTextChange()&&n),n},null,null)),n.Qa(13,1196032,null,0,h.a,[h.b,n.k,n.z],null,{event:"cdkObserveContent"}),n.ab(null,0)],function(t,e){var l=e.component;t(e,10,0,!0,23,t(e,11,0,150),l.disableRipple||l.disabled,n.bb(e,4))},function(t,e){var l=e.component;t(e,5,0,!n.bb(e,12).textContent||!n.bb(e,12).textContent.trim()),t(e,6,0,l.inputId,l.required,l.tabIndex,l.checked,l.disabled,l.name,l.ariaLabel,l.ariaLabelledby),t(e,9,0,n.bb(e,10).unbounded)})}var C=function(){function t(){}return t.prototype.ngOnInit=function(){},t}(),R=n.Pa({encapsulation:0,styles:[[""]],data:{}});function _(t){return n.kb(0,[(t()(),n.Ra(0,0,null,null,33,"div",[["fxLayout","column"],["fxLayoutAlign","start start"]],null,null,null,null,null)),n.Qa(1,737280,null,0,d.e,[c.g,n.k,c.k],{layout:[0,"layout"]},null),n.Qa(2,737280,null,0,d.d,[c.g,n.k,[6,d.e],c.k],{align:[0,"align"]},null),(t()(),n.ib(-1,null,["\n  "])),(t()(),n.Ra(4,0,null,null,28,"div",[],null,null,null,null,null)),(t()(),n.ib(-1,null,["\n    "])),(t()(),n.Ra(6,0,null,null,7,"mat-card",[["class","mat-card"]],null,null,null,b.b,b.a)),n.Qa(7,49152,null,0,g.a,[],null,null),(t()(),n.ib(-1,0,["\n      "])),(t()(),n.Ra(9,0,null,0,3,"mat-slide-toggle",[["class","mat-slide-toggle"]],[[8,"id",0],[2,"mat-checked",null],[2,"mat-disabled",null],[2,"mat-slide-toggle-label-before",null],[2,"_mat-animation-noopable",null]],null,null,S,w)),n.fb(5120,null,k.j,function(t){return[t]},[m.b]),n.Qa(11,1228800,null,0,m.b,[n.k,y.a,x.g,n.h,[8,null],n.z,m.a,[2,v.a],[2,Z.b]],null,null),(t()(),n.ib(-1,0,["Notifications"])),(t()(),n.ib(-1,0,["\n    "])),(t()(),n.ib(-1,null,["\n    "])),(t()(),n.Ra(15,0,null,null,7,"mat-card",[["class","mat-card"],["style","margin-top:10px;"]],null,null,null,b.b,b.a)),n.Qa(16,49152,null,0,g.a,[],null,null),(t()(),n.ib(-1,0,["\n      "])),(t()(),n.Ra(18,0,null,0,3,"mat-slide-toggle",[["class","mat-slide-toggle"]],[[8,"id",0],[2,"mat-checked",null],[2,"mat-disabled",null],[2,"mat-slide-toggle-label-before",null],[2,"_mat-animation-noopable",null]],null,null,S,w)),n.fb(5120,null,k.j,function(t){return[t]},[m.b]),n.Qa(20,1228800,null,0,m.b,[n.k,y.a,x.g,n.h,[8,null],n.z,m.a,[2,v.a],[2,Z.b]],null,null),(t()(),n.ib(-1,0,["Limit 4G Data usage"])),(t()(),n.ib(-1,0,["\n    "])),(t()(),n.ib(-1,null,["\n    "])),(t()(),n.Ra(24,0,null,null,7,"mat-card",[["class","mat-card"],["style","margin-top:10px;"]],null,null,null,b.b,b.a)),n.Qa(25,49152,null,0,g.a,[],null,null),(t()(),n.ib(-1,0,["\n      "])),(t()(),n.Ra(27,0,null,0,3,"mat-slide-toggle",[["class","mat-slide-toggle"]],[[8,"id",0],[2,"mat-checked",null],[2,"mat-disabled",null],[2,"mat-slide-toggle-label-before",null],[2,"_mat-animation-noopable",null]],null,null,S,w)),n.fb(5120,null,k.j,function(t){return[t]},[m.b]),n.Qa(29,1228800,null,0,m.b,[n.k,y.a,x.g,n.h,[8,null],n.z,m.a,[2,v.a],[2,Z.b]],null,null),(t()(),n.ib(-1,0,["Toogle dark mode (page reload needed)"])),(t()(),n.ib(-1,0,["\n    "])),(t()(),n.ib(-1,null,["\n  "])),(t()(),n.ib(-1,null,["\n"])),(t()(),n.ib(-1,null,["\n"]))],function(t,e){t(e,1,0,"column"),t(e,2,0,"start start")},function(t,e){t(e,9,0,n.bb(e,11).id,n.bb(e,11).checked,n.bb(e,11).disabled,"before"==n.bb(e,11).labelPosition,"NoopAnimations"===n.bb(e,11)._animationMode),t(e,18,0,n.bb(e,20).id,n.bb(e,20).checked,n.bb(e,20).disabled,"before"==n.bb(e,20).labelPosition,"NoopAnimations"===n.bb(e,20)._animationMode),t(e,27,0,n.bb(e,29).id,n.bb(e,29).checked,n.bb(e,29).disabled,"before"==n.bb(e,29).labelPosition,"NoopAnimations"===n.bb(e,29)._animationMode)})}var j=n.Na("app-settings",C,function(t){return n.kb(0,[(t()(),n.Ra(0,0,null,null,1,"app-settings",[],null,null,null,_,R)),n.Qa(1,114688,null,0,C,[],null,null)],function(t,e){t(e,1,0)},null)},{},{},[]),L=l("Ip0R"),B=l("eDkP"),O=l("4tE/"),z=l("o3x0"),D=l("jQLj"),E=l("mVsa"),U=l("uGex"),A=l("v9Dh"),M=l("4epT"),I=l("OkvK"),Q=l("wmQ5"),N=l("coki"),P=l("t/Na"),q=l("h1+l"),Y=l("6CRC"),T=l("mEDs"),V=l("nMkv"),F=l("hUWP"),G=l("V9q+"),H=l("4c35"),W=l("qAlS"),X=l("UodH"),K=l("u7R8"),J=l("de3e"),$=l("/dO6"),tt=l("LC5p"),et=l("YhbO"),lt=l("jlZm"),nt=l("seP3"),at=l("r43C"),it=l("SMsm"),ot=l("/VYK"),rt=l("b716"),st=l("0/Q6"),ut=l("Z+uX"),dt=l("Blfk"),ct=l("9It4"),bt=l("Nsh5"),gt=l("w+lc"),mt=l("vARd"),ht=l("Lwpp"),pt=l("y4qS"),ft=l("BHnd"),Zt=l("La40"),yt=l("8mMr"),vt=l("vvyD"),kt=l("PCNd"),xt=l("ZYCi"),wt={title:"Settings"},St=function(){},Ct=l("YSh2");l.d(e,"SettingsModuleNgFactory",function(){return Rt});var Rt=n.Oa(a,[],function(t){return n.Ya([n.Za(512,n.j,n.Da,[[8,[i.a,o.b,o.a,r.a,s.a,s.b,u.a,j]],[3,n.j],n.x]),n.Za(4608,L.m,L.l,[n.u,[2,L.w]]),n.Za(4608,c.i,c.h,[c.d,c.f]),n.Za(5120,n.b,function(t,e){return[c.l(t,e)]},[L.d,n.B]),n.Za(4608,B.a,B.a,[B.g,B.c,n.j,B.f,B.d,n.r,n.z,L.d,Z.b]),n.Za(5120,B.h,B.i,[B.a]),n.Za(5120,O.a,O.b,[B.a]),n.Za(4608,h.c,h.c,[]),n.Za(4608,f.d,f.d,[]),n.Za(5120,z.c,z.d,[B.a]),n.Za(4608,z.e,z.e,[B.a,n.r,[2,L.g],[2,z.b],z.c,[3,z.e],B.c]),n.Za(4608,D.h,D.h,[]),n.Za(5120,D.a,D.b,[B.a]),n.Za(5120,E.b,E.g,[B.a]),n.Za(4608,f.c,f.z,[[2,f.h],y.a]),n.Za(5120,U.a,U.b,[B.a]),n.Za(5120,A.a,A.b,[B.a]),n.Za(5120,M.b,M.a,[[3,M.b]]),n.Za(4608,p.f,f.e,[[2,f.i],[2,f.n]]),n.Za(5120,I.b,I.a,[[3,I.b]]),n.Za(4608,Q.a,Q.a,[]),n.Za(5120,N.d,N.b,[[3,N.d],P.c]),n.Za(4608,q.a,q.a,[Y.a,T.a,V.a]),n.Za(1073742336,L.c,L.c,[]),n.Za(1073742336,c.e,c.e,[]),n.Za(1073742336,Z.a,Z.a,[]),n.Za(1073742336,d.c,d.c,[]),n.Za(1073742336,F.b,F.b,[]),n.Za(1073742336,G.a,G.a,[[2,c.j],n.B]),n.Za(1073742336,f.n,f.n,[[2,f.f]]),n.Za(1073742336,y.b,y.b,[]),n.Za(1073742336,f.y,f.y,[]),n.Za(1073742336,f.w,f.w,[]),n.Za(1073742336,f.t,f.t,[]),n.Za(1073742336,H.g,H.g,[]),n.Za(1073742336,W.b,W.b,[]),n.Za(1073742336,B.e,B.e,[]),n.Za(1073742336,O.c,O.c,[]),n.Za(1073742336,X.c,X.c,[]),n.Za(1073742336,K.a,K.a,[]),n.Za(1073742336,g.e,g.e,[]),n.Za(1073742336,h.d,h.d,[]),n.Za(1073742336,J.c,J.c,[]),n.Za(1073742336,$.b,$.b,[]),n.Za(1073742336,z.k,z.k,[]),n.Za(1073742336,x.a,x.a,[]),n.Za(1073742336,D.i,D.i,[]),n.Za(1073742336,tt.b,tt.b,[]),n.Za(1073742336,et.c,et.c,[]),n.Za(1073742336,lt.a,lt.a,[]),n.Za(1073742336,nt.e,nt.e,[]),n.Za(1073742336,f.o,f.o,[]),n.Za(1073742336,at.b,at.b,[]),n.Za(1073742336,it.b,it.b,[]),n.Za(1073742336,ot.c,ot.c,[]),n.Za(1073742336,rt.b,rt.b,[]),n.Za(1073742336,st.c,st.c,[]),n.Za(1073742336,E.e,E.e,[]),n.Za(1073742336,f.A,f.A,[]),n.Za(1073742336,f.q,f.q,[]),n.Za(1073742336,U.c,U.c,[]),n.Za(1073742336,A.c,A.c,[]),n.Za(1073742336,M.c,M.c,[]),n.Za(1073742336,ut.a,ut.a,[]),n.Za(1073742336,dt.c,dt.c,[]),n.Za(1073742336,ct.a,ct.a,[]),n.Za(1073742336,bt.h,bt.h,[]),n.Za(1073742336,m.c,m.c,[]),n.Za(1073742336,gt.a,gt.a,[]),n.Za(1073742336,mt.d,mt.d,[]),n.Za(1073742336,I.c,I.c,[]),n.Za(1073742336,ht.d,ht.d,[]),n.Za(1073742336,Q.b,Q.b,[]),n.Za(1073742336,pt.o,pt.o,[]),n.Za(1073742336,ft.a,ft.a,[]),n.Za(1073742336,Zt.a,Zt.a,[]),n.Za(1073742336,yt.b,yt.b,[]),n.Za(1073742336,vt.a,vt.a,[]),n.Za(1073742336,kt.a,kt.a,[]),n.Za(1073742336,xt.p,xt.p,[[2,xt.v],[2,xt.l]]),n.Za(1073742336,St,St,[]),n.Za(1073742336,N.a,N.a,[]),n.Za(1073742336,a,a,[]),n.Za(256,$.a,{separatorKeyCodes:[Ct.f]},[]),n.Za(256,f.g,f.k,[]),n.Za(1024,xt.j,function(){return[[{path:"",component:C,data:wt}]]},[])])})}}]);