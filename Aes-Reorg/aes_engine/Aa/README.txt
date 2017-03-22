This is supposed to be a functional implementation of
an AES encryption/decryption unit developed by Shri Anshul Gupta
basically guided by MPD.

But what Anshul forgot to do is to document what he had done.
Of course this was to be expected.

So MPD needs to document this because it seems to matter to him.
Lets try to understand what Mr. Moron Gupta has done but forgotten
to document.

c_block.aa
	This seems to read from the cmd_in pipe and based on the command,
	do either encryption or decryption.
	
d_block.aa
	This seems to be a decryption block.  See comments in the file.
	
dec_round.aa
	Single decryption round.

e_block.aa
	This seems to be an encryption block.  See comments in the file.

enc_round.aa
	Encoding round. See comments in file.

invsbox.aa
	Inverse SBOX. See comments in file.

key_expnd.aa
	Key expansion. See comments in file.

sbox.aa
	SBOX. See comments in file.

wrapper.aa
 	Seems to be a wrapper of sorts. See comments in the file.

README.txt
	This file.
