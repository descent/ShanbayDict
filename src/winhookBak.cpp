#include "winhook.h"
#include <QApplication>
#include <windows.h>
#include <QPoint>
#include <QTime>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QList>
#include <QUrl>
#include "dict.h"
namespace WINHOOK{
HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;
QTime sct,dct;//sct single click time;dct double click time
QPoint downPos,upPos;
const int CLKINTERVAL=150;//����ʱ����
const int DBLCLKINTERVAL=300;//˫��ʱ����
//����ж�غ���,�Ա����
void unHook();
void setHook();
//���̹��ӹ���
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam ){


    //��WH_KEYBOARD_LLģʽ��lParam ��ָ��KBDLLHOOKSTRUCT���͵�ַ
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    if(pkbhs->vkCode==VK_F12)
    {
        unHook();
        qApp->quit();
    }
    return 1;//����1��ʾ��ȡ��Ϣ���ٴ���,����0��ʾ��������,��Ϣ��������

}
QMimeData *backupClipData;
bool clipExistData=false;
void backupClipboard(){
    //���ݼ�����
    const QMimeData *clipData=qApp->clipboard()->mimeData();
    backupClipData=new QMimeData();
    if(clipData->hasText()){
        //qDebug()<<"oldճ��������"<<qApp->clipboard()->text();
        backupClipData->setText(clipData->text());
        clipExistData=true;
    }
    if(clipData->hasColor()){
        backupClipData->setColorData(clipData->colorData());
        clipExistData=true;
    }
    if(clipData->hasHtml()){
        backupClipData->setHtml(clipData->html());
        clipExistData=true;
    }
    if(clipData->hasImage()){
        backupClipData->setImageData(clipData->imageData());
        clipExistData=true;
    }
    if(clipData->hasUrls()){
        backupClipData->setUrls(clipData->urls());
        clipExistData=true;
    }

}
bool clearClipboard(){
    bool clearok=false;
    if(OpenClipboard(NULL)){
        EmptyClipboard();
        if(CloseClipboard()){
            clearok=true;
            //qDebug()<<"close������success";
        }else{
            clearok=false;
            //qDebug()<<"close������failed";
        }
    }
    return clearok;
}

void restoreClipboard(){
    //�ָ�������
    if(clipExistData){
        qDebug()<<"restore clipboard";
        //      qApp->clipboard()->clear();//û����һ�䣬ճ����ָ����γ���ͻ�Ī������ı�����������OleFlushClipboard API�Ľ���
        //���ܼ���clear�������򼸺��������ˣ���ż�������QClipboard::clear: Failed to clear data on clipboard�����
        //        if (::OleSetClipboard(NULL) != S_OK) {
        //            qErrnoWarning("QClipboard::clear: Failed to clear data on clipboard");
        //            return;
        //        }

        if(clearClipboard()){
            qDebug()<<"�ָ�������success";
            qApp->clipboard()->setMimeData(backupClipData);//�ָ�����������
        }else{
            qDebug()<<"�ָ�������ʧ��";
        }

    }
}

void getSelectedText(){
    //unHook();
    backupClipboard();

    //ģ�ⷢ��ctrl+c����ѡ���text��ճ����
    qDebug()<<"׼��selected to clipboard";
    //qApp->clipboard()->clear();

    if(clearClipboard()){
        POINT pt;
        GetCursorPos(&pt);//��õ�ǰ���λ��
        HWND hwnd=WindowFromPoint(pt);//���������ڵĴ��ھ��
        QChar buffer[128];
        GetClassName(hwnd,(WCHAR*) buffer, 128);
        QString winclassname=QString::fromRawData(buffer,128);
        qDebug()<<"��ǰ����"<<hwnd<<winclassname;
        if(winclassname.contains("Internet Explorer")){//ie�Լ�ie�ں˵����������360��ȫ���������֧�ַ���ctrl+c������ʱ���У�����˫���ǿ��ԣ�
            qDebug("ie�ں�");
            PostMessage(hwnd,WM_COMMAND,0x0001000f,0);//ʹ��spy�ػ����Ϣ
//        }else if(winclassname.contains("TXGuiFoundation")){
//            qDebug()<<"QQ���촰��";
//            SendMessage(hwnd,WM_CHAR,49,49);

        }else{
            //    keybd_event(VK_CONTROL, 0, 0, 0); // ���� Ctrl
            //    keybd_event('C', 0, 0, 0); // ���� C
            //    keybd_event('C', 0, KEYEVENTF_KEYUP, 0); // �ſ� C
            //    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); // �ſ� Ctrl
           // int bScan=::MapVirtualKey('C',0);
           // qDebug()<<0x1D<<0x2e<<::MapVirtualKey(VK_CONTROL,0)<<::MapVirtualKey('C',0);
            //SendMessage(hwnd,WM_CAPTURECHANGED,0x0,0x0);
            keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_EXTENDEDKEY, 1); // ���� Ctrl
            keybd_event('C', 0x2E, 0,1); // ���� C
            //Sleep(10);
            //PostMessage(hwnd,WM_PAINT,0x0,0x0);//ʹ��spy�ػ����Ϣ

            keybd_event('C', 0x2E, KEYEVENTF_KEYUP, 1); // �ſ� C
            keybd_event(VK_CONTROL,0x1D, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 1); // �ſ� Ctrl
        }


        int iTimeOut=0;
        while (!qApp->clipboard()->mimeData()->hasText()){//�жϼ����������Ƿ�Ϊ���ָ�ʽ
            Sleep(10);
            qApp->processEvents();
            qDebug()<<"wait clip ok"<<iTimeOut<<qApp->clipboard()->text();
            iTimeOut ++;
            if (iTimeOut > 2){
                break;
            }

        }
        //qDebug()<<"wait clip ok"<<iTimeOut;
        if(qApp->clipboard()->mimeData()->hasText()){

            //qDebug()<<"ճ��������"<<qApp->clipboard()->text();
            DICT::querySelectedText(qApp->clipboard()->text());
        }
    }
    restoreClipboard();

   // setHook();
}

//��깳�ӹ���
//bool isruning=false;
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam ){

    // qDebug()<<"hello hook"<<nCode<<"wParam"<<wParam;
    if (nCode>=0)
    {
        //        if(isruning){
        //            qDebug()<<"��깳�������ˡ�����������������������������";
        //            return 1;
        //        }
        //isruning=true;
        if(wParam==WM_LBUTTONDOWN){
            sct.restart();
            downPos=QCursor::pos();

        }
        if(wParam==WM_LBUTTONUP){//can't get WM_LBUTTONDBLCLK,so let's code
            // qDebug()<<"single clicked time"<<sct.elapsed()<<"double clicked time"<<dct.elapsed();;
            if(sct.elapsed()<CLKINTERVAL){
                if(dct.elapsed()<DBLCLKINTERVAL){
                    //qDebug("double click");
                    getSelectedText();
                }
                dct.restart();
            }else{
                upPos=QCursor::pos();
                //qDebug()<<upPos.x()-downPos.x()<<upPos.y()-downPos.y();
                if(qAbs(upPos.x()-downPos.x())>6||qAbs(upPos.y()-downPos.y())>10){
                    //qDebug("mouse move");
                    getSelectedText();
                }
            }
        }
        // isruning=false;

    }
    // ����ϵͳ�е���һ������
    return CallNextHookEx(NULL, nCode, wParam, lParam);

}
//ж�ع���
void unHook(){
    //UnhookWindowsHookEx(keyHook);
    UnhookWindowsHookEx(mouseHook);

}
//��װ����,���øú�������װ����
void setHook(){
    //�������ײ㹳��,��ҪDLL�Ϳ���ȫ��
    //                         �ײ���̹���
    //keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
    //                          �ײ���깳��
    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
//    if(mouseHook){
//        qDebug()<<"���ӳɹ���������������������������������������������";
//    }else{
//        qDebug()<<"����ʧ��XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
//    }

}
}
