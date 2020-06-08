# apache_log_parser

## 概要
apache のログファイルを読み，1 時間単位でアクセスが多かった順にリモートホスト名とアクセス件数を数件表示します．

## 使い方
`apache_log_parser [-t time] [-b date] [-e date] [-n num] file...`

## オプション

-t time  
&nbsp; time に指定された時間のみを対象に表示します．  
&nbsp; デフォルトでは 24 時間分のデータがすべて表示されます．  

-b date  
(-e date)  
&nbsp; date に指定された年月日を集計の開始(終了)に設定します．  
&nbsp; date は YYYY/MM/DD 形式として受け付けます．  
&nbsp; デフォルトでは入力されたファイルの全データの総計を表示します．  

-n num  
&nbsp; num に指定された件数だけリモートホスト名とアクセス件数を表示します．  
&nbsp; デフォルトでは 10 件表示されます．


