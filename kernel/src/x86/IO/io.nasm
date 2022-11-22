extern port_outb

port_outb:
	pop eax ; 2nd arg to al (value)
	pop dx ; 1st arg to dx (port)
	out dx, eax