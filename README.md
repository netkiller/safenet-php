safenet-php
===========

SafeNet for PHP - http://www.safenet-inc.com/ 

Installation
------------
     # git clone https://github.com/netkiller/safenet-php.git
     # cd safenet-php
     # /srv/php-5.2.17/bin/phpize
     # ./configure --with-php-config=/srv/php-5.2.17/bin/php-config
     # make
     # make install
     
     # cat > /srv/php-5.2.17/etc/conf.d/safenet.ini <<PHP
     extension=safenet.so
     [SafeNet]
     safenet.url=http://175.145.40.55/safe/interface
     safenet.key=Web01-key
     PHP

Test
----
    # /srv/php-5.2.17/bin/php -m | grep safenet
    safenet
    
Code Example:
-------------
    <?php
    echo safenet_encrypt('helloworld');
    echo safenet_decrypt(safenet_encrypt('helloworld'));
