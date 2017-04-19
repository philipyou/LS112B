    @data = ();
    $MAX_CODE_PACKET_LEN = 0xF0;
    $MAX_MEM_PACKET_LEN = 0xf0;
    $i = 0;
    @mem_patch = ();
    $mem_patch_len = 0;
    
    
		##############################
		####Get All Data##############
		##############################
    open(EEP,"eeprom.dat") or die $!;
    while (<EEP>) {
    	$data[$i ++] = hex();
    	#printf "%x ",$data[$i-1]; #DEBUG;
    }
    close(EEP);
    
    
    open(BINFILE, ">bt_patch.bin");
    binmode(BINFILE);
		##############################
		####Calc Output Size##########
		##############################
    $fileSize = @data;
    $blocksCount = 0;
    $outputSize = 0;
    $codeBlockSize = 0;
    $fileOffset = $data[0] + $data[1];
    while($fileOffset < ($fileSize - 1)){
    	if(0xaa == $data[$fileOffset] || 0x55 == $data[$fileOffset + 1]){
    		$blockSize = $data[$fileOffset + 2] + ($data[$fileOffset + 3] << 8);
    		$outputSize += $blockSize;
    		if(0 == $blocksCount){
    			$codeCmdPacketCnt = int($blockSize / $MAX_CODE_PACKET_LEN);
					if(0 != $blockSize % $MAX_CODE_PACKET_LEN){
			    	$codeCmdPacketCnt ++;
			    }
			    $outputSize += 9 * $codeCmdPacketCnt;
    			$fileOffset += $blockSize + 4;
    			#printf "DEBUG:blockSize:%d blocksCount: %d,fileOffset:%d,codeCmdPacketCnt:%d,outputSize:%d\n",$blockSize, $blocksCount,$fileOffset,$codeCmdPacketCnt,$outputSize;
    		}elsif(1 == $blocksCount){
    			$fileOffset += $blockSize + 4;
    		}else{
    			
	  			$memPacketCnt = int($blockSize / $MAX_MEM_PACKET_LEN);
	  			if(0 != $blockSize % $MAX_MEM_PACKET_LEN){
	  				$memPacketCnt ++;
	  			}
	  			$outputSize += 8 * $memPacketCnt;
	  			$fileOffset += $blockSize + 6;
	  			#printf "DEBUG:blockSize:%d blocksCount: %d,fileOffset:%d,memPacketCnt:%d,outputSize:%d\n",$blockSize, $blocksCount,$fileOffset,$memPacketCnt,$outputSize;
    		}
    		$blocksCount ++;
    	}else{
    		if($fileOffset == $fileSize - 2){
    			$fileOffset += 2;
    		}else{
	    		printf "ERROR:blockSize:%d blocksCount: %d\n",$blockSize, $blocksCount;
	    		printf "ERROR:fileOffset:%d Block data: %x %x\n",$fileOffset, $data[$fileOffset], $data[$fileOffset + 1];
	    		die "ERROR:Block header \"aa55\" Error while Calc output size!\n";
    		}
    	}
    }
		
    $outputSize +=  40;
    printf BINFILE "%c", ($outputSize & 0xff);#offset low
		printf BINFILE "%c", (($outputSize >> 8) & 0xff);#offset High
		
		
		##############################
		####Start Generate Bin########
		##############################
		$blocksCount = 0;
    $fileOffset = $data[0] + $data[1];
    while($fileOffset < ($fileSize - 1)){ #file was endof a 0xff, ignore it.
    	#printf "DEBUG:Process a new block:%d\n", $blocksCount;
    	if(0 == $blocksCount){#bt code block 
    		##############################
    		####Process bt code block!####
    		##############################
    		if(0xaa != $data[$fileOffset] || 0x55 != $data[$fileOffset + 1]){
    			printf "ERROR:fileOffset = %d\n Block data: %x %x\n",$fileOffset, $data[$fileOffset], $data[$fileOffset + 1];
    			die "ERROR:Block header \"aa55\" Error Code 0x01!\n";
    		}
    		#printf "DEBUG:fileOffset:%x, data+2:%x, data+3:%x\n",$fileOffset, $data[$fileOffset + 2], $data[$fileOffset + 3];
    		$codeSize = $data[$fileOffset + 2] + ($data[$fileOffset + 3] << 8);
    		$codeStart = $fileOffset + 4;#aa 55 LL LH
    		$codeEnd = $codeStart + $codeSize;
    		$codeoffset = $codeStart;
    		
    		#printf "DEBUG:Begain to Process bt code block.\n";
    		#printf "DEBUG:codeSize:%x, codeoffset:%x, codeEnd:%x\n",$codeSize, $codeoffset, $codeEnd;
    		#printf "DEBUG:codeEnd data:%x\n", $data[$codeEnd];

    		while($codeoffset < $codeEnd){
    			
	    		$packetLen = $MAX_CODE_PACKET_LEN;
	    		if($codeoffset + $packetLen > $codeSize){
	    			$packetLen = $codeEnd - $codeoffset;
	    		}
	    		#printf "DEBUG:new packet codeoffset:%x, length:%x\n", $codeoffset, $packetLen;
	    		$packetEnd = $codeoffset + $packetLen;
	    		printf BINFILE "%c", (($packetLen + 8) & 0xff);#whole packet len;
					printf BINFILE "%c%c%c", 0x01, 0x03, 0xfc;
					printf BINFILE "%c", (($packetLen + 4) & 0xff);#packet len;
					printf BINFILE "%c", (($codeoffset - 6) & 0xff);#offset low
					printf BINFILE "%c", ((($codeoffset - 6) >> 8) & 0xff);#offset High
					printf BINFILE "%c", (($packetLen) & 0xff);#code len;
					$checkSum = ($codeoffset - 6) + (($codeoffset - 6) >> 8) + ($packetLen);
	    		while($codeoffset < $packetEnd){
	    			#printf "%02x ", $data[$codeoffset];#DEBUG
	    			printf BINFILE "%c", ($data[$codeoffset] & 0xff);#code;
	    			$checkSum += $data[$codeoffset];
	    			$codeoffset ++;
	    		}
	    		printf BINFILE "%c", ($checkSum & 0xff);#check sum;
	    		#printf "DEBUG: Packet End.\n"
    		}
    		
    		$fileOffset = $codeEnd;
    		##############################
    		####End of bt code block  ####
    		##############################
    		
    	}elsif(1 == $blocksCount){#c51 code block 
    		##############################
    		####Process C51 code block####
    		##############################
    		if(0xaa != $data[$fileOffset] || 0x55 != $data[$fileOffset + 1]){
    			printf "ERROR:fileOffset = %d\n Block data: %x %x\n",$fileOffset, $data[$fileOffset], $data[$fileOffset + 1];
    			die "ERROR:Block header \"aa55\" Error Code 0x02\n";
    		}
    		
    		$fileOffset += 4;
    		##############################
    		####End of C51 code block ####
    		##############################
    	}else{
    		##############################
    		####Process a memory block####
    		##############################
    		if(0xaa != $data[$fileOffset] || 0x55 != $data[$fileOffset + 1]){
    			if($fileOffset == $fileSize - 2){
    				$fileOffset += 2;
    			}else{
    				printf "ERROR:fileSize = %d\n", $fileSize;
    				printf "ERROR:fileOffset = %d\n Block data: %x %x\n",$fileOffset, $data[$fileOffset], $data[$fileOffset + 1];
    				die "ERROR:Block header \"aa55\" Error Code 0x03\n";
    			}
    		}
    		$memBlockLen = $data[$fileOffset + 2] + ($data[$fileOffset + 3] << 8);
    		$memBlockAddr = $data[$fileOffset + 4] + ($data[$fileOffset + 5] << 8);
  			$memBlockBreakCnt = int($memBlockLen / $MAX_MEM_PACKET_LEN);
  			if(0 != $memBlockLen % $MAX_MEM_PACKET_LEN){
  				$memBlockBreakCnt ++;
  			}

  			#printf "DEBUG:memBlock, memBlockLen:%d, memBlockAddr:0x%x, memBlockBreakCnt:%d\n",$memBlockLen,$memBlockAddr,$memBlockBreakCnt ;
  			if(0x4000 == $memBlockAddr){
    			$mem_patch_len = $memBlockLen;
    			#printf "DEBUG:Find mem_patch, length = %d\n",$mem_patch_len;
    		}
    		
  			$j = 0;
  			$memOffset = 0;                     #mem data offset in binfile
  			$currentPacketAddr = $memBlockAddr;  #addr write in packet
  			$memBlockStart = $fileOffset + 6;   #mem data start in binfile
  			
  			while($j++ < $memBlockBreakCnt){
  				#for one single packet.
  				$currentPacketLen = $MAX_MEM_PACKET_LEN;
  				if($memOffset + $currentPacketLen > $memBlockLen){
    				$currentPacketLen = $memBlockLen - $memOffset;#last packet
    			}
    			
    			#printf "DEBUG:memPacket, currentPacketLen:%d, currentPacketAddr:0x%x, memOffset:%d\n",$currentPacketLen,$currentPacketAddr,$memOffset ;
    			
  				#packet header
  				printf BINFILE "%c", (($currentPacketLen + 7) & 0xff);
					printf BINFILE "%c%c%c", 0x01, 0x10, 0xfc;
					printf BINFILE "%c", (($currentPacketLen + 3) & 0xff);
					printf BINFILE "%c", ($currentPacketLen & 0xff);
					printf BINFILE "%c", ($currentPacketAddr & 0xff);
					printf BINFILE "%c", (($currentPacketAddr >> 8) & 0xff);
					
					#copy data
					$i = 0;
					while($i < $currentPacketLen){
						printf BINFILE "%c",($data[$memBlockStart + $memOffset + $i] & 0xff);
						if(0x4000 == $memBlockAddr){
							$mem_patch[$i] = ($data[$memBlockStart + $memOffset + $i] & 0xff)
						}
						$i++;
					}
					
					#endof one packet
  				$currentPacketAddr += $i;
  				$memOffset += $i;
  				
  			}
  			#endof a mem block
  			$fileOffset = ($memBlockStart + $memOffset);
  			##############################
  			####End of a memory block#####
  			##############################
	    		
    	}
    	$blocksCount ++;
    }
    #printf "DEBUG:fileOffset:%d, fileSize:%d", $fileOffset, $fileSize;
    #printf "DEBUG:End of block process.Memory block count:%d\n", ($blocksCount - 2);
    if(0x40 != $mem_patch_len){
    	printf "ERROR:mem_patch_len:%d",$mem_patch_len;
    	die "ERROR:Cannot find mem_patch block or length error.";
    }
    
    printf BINFILE "%c%c%c%c%c", 0x25, 0x01, 0x04, 0xfc, 0x21;
		$i = 0;
		$checksum = 0;
		while($i < 0x20){
			printf BINFILE "%c", ($mem_patch[$i] & 0xff);
			
			$checksum += ($mem_patch[$i] & 0xff);
			$i ++;
		}
		printf BINFILE "%c", ($checksum & 0xff);
		
		close(BINFILE);
		
		
		
		
		
		
