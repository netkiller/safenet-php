safenet-php
===========

SafeNet for PHP - http://www.safenet-inc.com/ 

Installation
------------
     # yum install -y libcurl-devel

     # git clone https://github.com/netkiller/safenet-php.git
     # cd safenet-php
     # /srv/php/bin/phpize
     # ./configure --with-php-config=/srv/php/bin/php-config
     # make
     # make install
     
     # cat > /srv/php/etc/conf.d/safenet.ini <<PHP
     extension=safenet.so
     [SafeNet]
     safenet.url=http://175.145.40.55/safe/interface
     safenet.key=Web01-key
     safenet.timeout=10
     PHP

Test
----
    # /srv/php/bin/php -m | grep safenet
    safenet

    php -r "echo safenet_encrypt('helloworld');"
    php -r "echo safenet_decrypt(safenet_encrypt('helloworld'));"

Code Example:
-------------
    <?php
    echo safenet_encrypt('helloworld');
    echo safenet_decrypt(safenet_encrypt('helloworld'));
