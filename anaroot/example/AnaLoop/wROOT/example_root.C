
//anafileを使わないROOTユーザー向け
//TArtAnaLoopクラスを継承したクラスを手元で編集して.Lでロードする。
//histogramはTArtAnaLoopUser内で自分で作成・管理
{
  gROOT->ProcessLine(".L macro/analoop/TArtAnaLoopUser.C+"); //自前のAnaLoop子クラスをロード
  book(new TArtAnaLoopUser); //ここでnewしたanaloopは内部で管理しているのでユーザーが勝手にdeleteしてはいけない
  push("ridf/data0199.ridf", 10000); // 解析するファイルを追加, 10000イベントのみ
  push("ridf/data0094.ridf"); // 解析するファイルを追加, 全イベント
  start(); // 解析開始
  push("ridf/data0201.ridf"); // いつでも(bookする前でも)追加可能
  status(); // イベント数等を表示
  pop(1); // 1番目にスタックされているファイルの解析は止める。
  status();

  //他にも、next()で今の解析をすっ飛ばして次の解析に行く(startの代わりに大体使える)。
}

