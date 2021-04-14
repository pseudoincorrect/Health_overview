import { ToastModule } from './toast.module';

describe('ToastModule', () => {
  let toastModule: ToastModule;

  beforeEach(() => {
    toastModule = new ToastModule();
  });

  it('should create an instance', () => {
    expect(toastModule).toBeTruthy();
  });
});
