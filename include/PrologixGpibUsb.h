/* 
 * File:   PrologixGpibUsb.h
 * Author: yoshiki
 *
 * Created on 2011/05/28, 16:06
 */

/**
 * \mainpage
 * 
 * Prologix社製GPIB-USBコントローラを使用するためのライブラリ。標準のAPIはそのままでも
 * そこそこ使いやすいものではあるが、さらに使いやすくするためのもの。
 * 
 * \section Prologix社製GPIB-USBコントローラ
 * \li GPIBインターフェースを持つ計測機器をUSBでパソコンと接続して制御することができる。
 * <UL>
 * <LI> GPIBの計測機器をパソコンに接続するには、インターフェースボード（PCIバスなど）を購入してパソコンに装着して使うものだったが、最近はUSBやイーサネットなどがより好まれる。またノートパソコンの場合にPCIバスだと困る。
 * <LI> USBなら、ほとんどのパソコンに接続できる。
 * <LI> 内部的には、この種のコントローラは、単なるコネクタ形状の変換をしているだけでなく、コマンドGPIB/USBのコマンド送受信のルール（プロトコル）の変換等を行う、そこそこ複雑な仕事をやってくれるものである。
 * </UL>
 * \li コントローラはFTDI (Figure Technology Devices International)社製の
 * チップを用いて作られているので、ドライバとAPIはFTDIが配布しており、ホームページから無料でダウンロード
 * できる。
 * <UL>
 * <LI> Prologix社のページ： http://prologix.biz/
 * <LI> FTDI社のページ： http://www.ftdichip.com/
 * </UL>
 * 
 * \subsection 他社製のGPIB-USBコントローラ
 * \li Prologix社以外にも、コンテック、ナショナルインスツルメンツなどが発売している。ただしコンテックの
 * ものはWindowsのみの対応。ナショナルインスツルメンツはWindows, Linux, MacOSのそれぞれに対応している
 * ものを発売しているが、別製品。同一製品で３つの全てに対応しているものはPrologix社の以外に
 * 知らない。
 * \li 破格の安さ：
 * <UL>
 * <LI> コンテック：¥44,000 （Windows用のみ）
 * <LI> ナショナルインスツルメンツ：¥43,000 (Windows用）http://sine.ni.com/nips/cds/view/p/lang/ja/nid/201586
 * <LI> ナショナルインスツルメンツ：¥61,000 (Linux用、MacOS用がそれぞれ別製品として）
 * <LI> Prologix: ¥16,000 （これ１台でWindows, Linux, MacOS 全対応）
 * </UL>
 * 
 * \subsection ドライバのダウンロードとインストール
 * <UL>
 * <LI> FTDI社のページ http://www.ftdichip.com/Drivers/D2XX.htm から使用するOSのD2XXドライバをダウンロード。
 * <LI> インストーラは用意されていないのでマニュアルで行う。(Windowsは試してない）
 * <UL>
 * <LI> Mac の場合
 * <UL>
 * <LI> ダウンロードしたファイルを展開：D2XX/ というディレクトリができる。あとはReadMe にしたがう（以下）
 * <LI> ライブラリファイル bin/10.5-10.6/libftd2xx.1.0.4.d7lib を/usr/local/lib にコピー (root権限必要、つまり sudo cp）
 * <LI> ライブラリファイルへのシンボリックリンク作成：sudo ln -sf /usr/local/lib/libftd2xx.1.0.4.dylib /usr/local/lib/libftd2xx.dylib
 * <LI> ヘッダファイル Samples/ftd2xx.h を /usr/local/include にコピー （root権限必要：つまり sudo cp）
 * <LI> ヘッダファイル Samples/WinTypes.h を /usr/local/include にコピー （root権限必要）
 * </UL>
 * <LI> Linuxの場合
 * <UL>
 * <LI> su で root になる
 * <LI> ライブラリファイルを /usr/local/lib にコピー。64bitのLinuxなら build/x86_64/ の下にある libftd2xx.a, libftd2xx.so.1.0.4。
 * <LI> シンボリックリンク作成：ln -s /usr/local/lib/libftd2xx.so.1.0.4 /usr/local/lib/libftd2xx.so
 * <LI> ヘッダファイル Samples/ftd2xx.h を /usr/local/include にコピー
 * <LI> ヘッダファイル Samples/WinTypes.h を /usr/local/include にコピー
 * </UL>
 * </UL>
 * </UL> 
 * </UL> 
 * 
 * \section このライブラリについて
 * \li FTDI社のページからドライバをダウンロードしてきたら、すでにGPIB-USBコントローラは使える/APIを使ってプログラミングできる状態にある。
 * \li このライブラリは、そのプログラミングをもっと楽にわかりやすくできるようにという目的で作られたものである。
 * \li もともとのAPIは比較的わかりやすくできているので、このライブラリによってどれだけ楽ができているかは、たいしたことないかもしれない。
 * 
 * \subsection GPIB機器を動かすためのプログラミングについて
 * \li 基本的に、そのようなプログラムは以下のような構造を持ち、それ以外にない：
 * <UL>
 * <LI> デバイスのオープン：機器とパソコンとの接続を確立し、使える状態にする
 * <LI> コマンドの送信：APIを介して、機器を動作させるためのコマンドを送りつける
 * <LI> 応答の受信：機器からの応答（データ）を受け取る
 * <LI> デバイスのクローズ：終了時には、機器とパソコンの接続を正しく解除し、平和に終わる。
 * </UL>
 * \li 上の２つの動作をさせるために、このライブラリでは ProligxGpibUsb というクラスを用意し、
 * <UL>
 * <LI> そのクラスのインスタンスの作成がデバイスのオープン:PrologixGpibUsb *device = new PrologixGpibUsb();
 * <LI> コマンド送信はメソッド device->SendCommand(cmd);
 * <LI> 機器からの応答（データ）受信は device->ReceiveResponse(data)
 * <LI> デバイスのクローズ：デストラクタ
 * </UL>
 * \li よってこのライブラリを使うには、まずは上の４つだけ知っておけばよい。
 * \li 後は、やりたいことに応じて機器のマニュアルを参照して、機能を実現するコマンドを探し、SendCommand()で送り、データを ReceiveReponse() で受け取る。
 * 
 * \section プログラム例
 * \li GPIBアドレス20に設定したテクトロニクス製デジタルオシロを動かすサンプル。ここでは
 * <UL>
 * <LI> チャンネル1の縦軸の刻みを50mV/divisionに
 * <LI> 横軸の刻みを 4μ秒/division に
 * <LI> 機器の名前を問い合わせて表示
 * </UL>
 * \verbatim
    #include "ProligixGpibUsb.h"
     
    int main(int argc, char *argv[]) {
      PrologixGpibUsb device = PrologixGpibUsb(20);   // GPIBアドレス：20
      device.SendCommand("CH1:SCALE 50e-3");  // チャンネル1の縦軸の刻みを50mV/divisionに
      device.SendCommand("HORIZONTAL:SCALE 4e-6"); // 横軸の刻みを 4μ秒/division に
      device.SendCommand("*IDN?");  // 機器の名前を問い合わせ
      char rBuf[256];  // 機器からの応答を受け取る箱。十分な長さを確保のこと
      device.ReceiveResponse(rBuf);  // 機器からの応答を受け取る
      printf("%s\n", rBuf);　　　　// 結果を表示
      return 0;
    }
   \endverbatim
 */
#ifndef PROLOGIXGPIBUSB_H
#define	PROLOGIXGPIBUSB_H

#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <ftd2xx.h>

class PrologixGpibUsb {
protected:
  FT_HANDLE ftHandle;  
  char controllerName[256];
  void SetDefault();
public:
  /**
   * コンストラクタ：Prologix社製GPIB-USBコントローラとパソコンとの接続を確立する。
   * このコンストラクタの後で、必ず SendCommand("++addr アドレス") で操作したい装置のGPIBアドレスを
   * 指定すること。または、GPIBアドレスを引数に取るコンストラクタを使ってもよい。つまり、以下の２つは同じ：
   * 
   * 1)
   *  \verbatim 
        PrologixGpibUsb *device = new PrologixGpibUsb();   
        device->SetGpibAddress(addr);
      \endverbatim
   * 
   * 2)
   *  \verbatim
        PrologixGpibUsb *deviec = new PrologixGpibUsb::PrologixGpibUsb(addr);
      \endverbatim
   */
  PrologixGpibUsb();
  /**
   * コンストラクタ：Prologix社製GPIB-USBコントローラとパソコンとの接続を確立する。接続の確立後、
   * これから操作する測定機器のGPIBアドレスを教えてやる。このコンストラクタは以下と同等である：
   * \verbatim
      PrologixGpibUsb *device = new PrologixGpibUsb(); 
      device->SetGpibAddress(addr);
     \endverbatim
   */  
  PrologixGpibUsb(int addr /*!< GPIB address */);  
  PrologixGpibUsb(const PrologixGpibUsb& orig);
  virtual ~PrologixGpibUsb();
  /**
   * 装置へコマンドを送りつける。コマンドを送る先の装置のGPIBアドレスは "++addr" コマンドで事前に指定しておく必要がある。
   * 送るコマンドの意味は、装置のプログラミングマニュアルを参照して調べておくこと。
   * なお "++" で始まるコマンドは、GPIB-USBコントローラへ送られ、測定装置には送られない。
   * 
   * @param wBuf Command 装置へ送付するコマンド。終端文字 ("\n") はつけなくてもよい（このメソッドの内部で付加される）。
   * @return 書き込んだコマンドの長さ
   */
  DWORD SendCommand(const char *wBuf);
  /**
   * GPIBアドレス addr を持つ装置へコマンドを送りつける。SetGpibAddress(addr) を呼び出してから SendCommand(wBuf) としても同じ。
   * @param addr コマンドを送りたい装置のGPIBアドレス
   * @param wBuf Command 装置へ送付するコマンド。終端文字 ("\n") はつけなくてもよい（このメソッドの内部で付加される）。
   * @return 書き込んだコマンドの長さ
   */
  DWORD SendCommand(int addr, const char *wBuf);
  /**
   * 装置からのレスポンス（データ）を受け取る。事前にSendCommand()でコマンド（"?"で終わるクエリ型コマンド）を送付していること。
   * @param rBuf 装置からのデータを受け取るための文字列バッファ。呼び出し型で用意のこと。
   * @return 読み込んだデータの長さ
   */
  DWORD ReceiveResponse(char rBuf[]);
 
  /**
   * まずクエリ型コマンド（"?"で終わる反応要求型コマンド）を送り、その出力を受け取る
   * @param cmd クエリコマンド
   * @param rBuf データを受け取る入れ物
   * @return 読み込んだデータの長さ
   */
  DWORD ReceiveResponse(const char *cmd /*!< Command to send */, char rBuf[] /*!< Response from the device */);
  /**
   * アドレス設定、コマンド送信と受信を合わせて行う。
   * @param addr コマンド送付先の装置のGPIBアドレス
   * @param cmd クエリコマンド
   * @param rBuf 受け取るデータを入れる文字列バッファ
   * @return 受け取ったデータの長さ
   */
  DWORD ReceiveResponse(int addr /*!< GPIB address */, const char *cmd /*!< command to send */, char rBuf[] /*!< Response from the device */);  
  /**
   * 以後のコマンドを送りつける先の装置のGPIBアドレスを設定する。
   * いったんこのメソッドでアドレスを設定すると、以後のSendCommand()やReceiveResponse()はその装置に対して行われる。
   * 別の装置へのコマンド送信や受信をするときは、あらためて対象装置のアドレスをこのメソッドで設定してから行う。
   * 
   * ex) GPIBアドレス 20 の装置を扱う
   * 
   *    device->SetGpibAddress(20);
   * 
   * なおこのメソッドは
   * 
   *    device->SendCommand("++addr アドレス");
   * 
   * と同等である。
   * @param addr 操作したい機器のGPIBアドレス
   */
  void SetGpibAddr(int addr /*!< GPIB address */);
  /**
   * デバイスのハンドラを返す。直接APIの関数 FT_XXX() を呼び出したいときは、このメソッドが返すハンドラを第１引数として使うことになる。
   * 
   * 注：このライブラリを作った意図は、なるべく FT_XXX() を直接呼び出さなくてもいいようにすることにある。このメソッドでハンドラを取り出す
   * 必要があるならば、最初からFTDIのAPIそのものを使ったプログラミングをしている方が一貫性は保たれる。このメソッドは存在するべきではないかもしれない。
   * @return デバイスハンドラ
   */
  FT_HANDLE GetHandle() const { return ftHandle; }
  
  /**
   * 測定機器に対してクリアコマンドを送る。送り先の測定機器は、その前に設定されているアドレスのもの。測定機器がどういう応答をするかは、測定機器のマニュアルで確認のこと。
   * なおこのメソッドは this->SendCommand("++clr"); と同等である。
   */
  void Clear() {
    this->SendCommand("++clr");
  }
  /**
   * 指定したGPIBアドレスの機器に対してクリアコマンドを送る。
   * @param addr クリア信号送信先の機器のGPIBアドレス
   */
  void Clear(int addr) {
    this->SetGpibAddr(addr);
    this->Clear();
  }  
  /**
   * 読み出し（機器からのレスポンス受け取り）および書き込み（コマンド送信）のタイムアウトをミリ秒で設定する。
   * 読み出しまたは書き込みがここで指定した時間以内に終了しない場合、強制的に打ち切られる。
   * @param dwReadTimeout 読み出し（機器からのレスポンス受け取り）のタイムアウト時間 [ms]
   * @param dwWriteTimeout 書き込み（コマンド送信）のタイムアウト時間 [ms]
   */
  FT_STATUS SetTimeouts(DWORD dwReadTimeout, DWORD dwWriteTimeout) {
    return FT_SetTimeouts(ftHandle, dwReadTimeout, dwWriteTimeout);
  }
  FT_STATUS SetFlowControl(USHORT usFlowControl, UCHAR uXon, UCHAR uXoff) {
    return FT_SetFlowControl(ftHandle, usFlowControl, uXon, uXoff);    
  }
  /**
   * Sets Data Terminal Ready (DTR) control signal
   * @return 
   */
  FT_STATUS SetDtr() {
    return FT_SetDtr(ftHandle);
  }
  /**
   * Clears Data Terminal Ready (DTR) control signal.
   * @return 
   */
  FT_STATUS ClrDtr() {
    return FT_ClrDtr(ftHandle);
  } 
  /**
   * Sets the Request To Send (RTS) control signal.
   * @return 
   */
  FT_STATUS SetRts() {
    return FT_SetRts(ftHandle);
  }
  /**
   * Clears the Request To Send (RTS) control signal.
   * @return 
   */
  FT_STATUS ClrRts() {
    return FT_ClrRts(ftHandle);
  }   
  /**
   * デバイスにリセットコマンドを送る
   */
  FT_STATUS ResetDevice() {
    return FT_ResetDevice(ftHandle);
  }
};

#endif	/* PROLOGIXGPIBUSB_H */

