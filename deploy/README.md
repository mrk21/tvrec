# Tvrec deployment documentation

## Make Ubuntu 14.04 LTS installation media

You enter the command listed below on the local:

    $ wget http://releases.ubuntu.com/14.04/ubuntu-14.04.1-desktop-amd64.iso
    $ sudo dd if=ubuntu-14.04.1-desktop-amd64.iso of=/dev/<sd-card-device> bs=1m

## Install and Settings

### Install Ubuntu 14.04 LTS

You connect the installation media before power on, and boot Ubuntu 14.04 LTS installation program. You execute the steps listed below when it was booted.

#### インストール: ようこそ

    言語: 日本語
    
    ( ) Ubuntu を試す
    (+) Ubuntu をインストール

#### インストール: Ubuntu のインストール準備

    [x] インストール中にアップデートをダウンロードする
    [x] サードパーティーのソフトウェアをインストールする
    
    ( ) 終了(Q)
    ( ) 戻る(B)
    (+) 続ける

#### インストール: インストールの種類

    ( ) xxx を Ubuntu 14.04 LTS と併用可能な形でインストール
    (+) xxx を削除して再インストール
      [ ] Encript the new Ubuntu installation for security
      [ ] User LVM with the new Ubuntu installation
    ( ) それ以外
    
    ( ) 終了(Q)
    ( ) 戻る(B)
    (+) 続ける

#### インストール: xxx を削除して再インストール

    ドライブの種類: xxx
    ディスディスク全体を使用する: xxx
    
    ( ) 終了(Q)
    ( ) 戻る(B)
    (+) インストール

#### インストール: どこに住んでいますか？

    場所: Tokyo
    
    ( ) 戻る(B)
    (+) 続ける

#### インストール: キーボードレイアウト

    キーボードレイアウトの選択: 日本語 -> 日本語
    
    ( ) 戻る(B)
    (+) 続ける

#### インストール: あなたの情報を入力して入力してください

    あなたの名前: xxx
    コンピュータの名前: tv
    ユーザー名の入力: xxx
    パスワードの入力: xxx
    パスワードの確認: xxx
    
    (+) 自動的にログインする
    ( ) ログイン時にパスワードを要求する
    
    [ ] ホームフォルダを暗号化する
    
    ( ) 戻る(B)
    (+) 続ける

#### インストールが完了しました

    (+) 今すぐ再起動する

#### Reboot

You press an enter key after ejecting the installation media:

    Please remove installation media and close the tray (if any) then press ENTER: 

### Network settings

システム設定 -> ネットワーク -> 有線 -> オプション -> IPv4設定

    方式(M): 手動
    
    (+) 追加
    ( ) 削除
    
    アドレス:
      アドレス: 192.168.1.x
      ネットマスク: 255.255.255.0
      ゲートウェイ: 192.168.1.x
    DNSサーバ(V): 192.168.1.x
    ドメインを検索(E): 
    DHCPクライアントID(H): 
    
    [ ] この接続を完了するにはIPv4アドレス化が必要になります(4)
    
    ( ) ルート(R)...
    
    ( ) キャンセル(C)
    (+) 保存(S)...

### Display settings

システム設定 -> 画面の明るさとロック

    次の時間アイドル状態が続けば画面をオフにする(T): しない
    
    ロックする: オフ
      画面ロックまでの時間(L): 
    
    [ ] サスペンドからの復帰時にパスワードを要求する

### Input Settings

システム設定 -> テキスト入力 -> 日本語(Anthy) -> 設定 -> 一般

    初期値の設定
      入力モード(I): 英数
      入力タイプ(T): ローマ字
      変換モード(M): 連文節

### Install SSH server

You enter the command listed below on the remote server:

    $ sudo apt-get install ssh

### Provisioning by Ansible

#### Set config files

You must set files listed below on the config directory:

    config
    ├── auth
    │   ├── basic_auth
    │   ├── ssl.crt
    │   ├── ssl.csr
    │   ├── ssl.key
    │   └── ssl.key.org
    ├── inventory.ini
    └── ssh
        ├── authorized_keys
        ├── config
        ├── id_rsa
        └── known_hosts

The following tools can generate the above `auth` files:

* tools/basic\_auth.sh
* tools/create\_cart.sh

##### basic\_auth.sh

This script generates `auth/basic_auth`.

    $ ./tools/basic_auth.sh <username>

##### create\_cart.sh

This script generates `auth/ssl.xxx`.

    $ ./tools/create_cart.sh

#### Run ansible playbook

You enter the command listed below on the local:

    $ rake

### Create Samba user

You enter the command listed below on the remote server:

    $ sudo pdbedit -a <user>

### Input VNC password

You enter the command listed below on the remote server:

    $ sudo x11vnc -storepasswd /etc/x11vnc.passwd

### XBMC Setting

    System -> Appearance -> Skin -> Fonts: Arial Base
    System -> Appearance -> International -> Language: Japanese
    System -> System -> Audio hardware -> Audio output: HDMI
    System -> Services -> Webserver -> Allow control of XBMC for HTTP: [x]
    System -> Video -> File -> Add video -> Media path: /media

### VLC Setting

    ツール -> 設定 -> ビデオ -> デインターレース: 自動

### Mediatomb Settings

1. Access `http://<hostname>:49152` by browser
2. Click to `Filesystem`
3. Select the directory of `Filesystem/media/video1`
4. Click the button of `add as autoscan dir` for the selected directory
5. Set the items as shown below:

        Scan Mode:
          [ ] None
          [ ] Timed
          [x] Inotify
        Initial Scan:
          [ ] Basic
          [x] Full
        Recursive: [x]
        Include hidden files/directories: [ ]

6. Repeat steps of `3` to `5` regarding the directory of `Filesystem/media/video2`
