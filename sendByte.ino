struct ControlByte {
  int sys,sub,isr;
  unsigned val;
  unsigned getCb(){
  unsigned ret = isr;
  ret += sub*16;
  // ret += sys*64;  
  val = ret;
  return ret;
  }
};

struct DataPacket {
  ControlByte controlByte;
  unsigned dat1,dat0,dec;
  
};


template <typename T>
Print& operator<<(Print& printer, DataPacket value)
{
  printer.write(value.controlByte.val);
  printer.write(value.dat1);
  printer.write(value.dat0);
  printer.write(value.dec);
  return printer;
}
