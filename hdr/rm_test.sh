#!/bin/bash 
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_hdr.pfm ]]; then
	rm $AG/HW1/CO417-HW1/Memorial/memorial_hdr.pfm
fi
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_hdr.ppm ]]; then
	rm $AG/HW1/CO417-HW1/Memorial/memorial_hdr.ppm
fi
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_tonemap.pfm ]]; then
	rm $AG/HW1/CO417-HW1/Memorial/memorial_tonemap.pfm
fi
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_tonemap.ppm ]]; then
	rm $AG/HW1/CO417-HW1/Memorial/memorial_tonemap.ppm
fi
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_tm_brighter.pfm ]]; then
    rm $AG/HW1/CO417-HW1/Memorial/memorial_tm_brighter.pfm
fi
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_tm_brighter.ppm ]]; then
    rm $AG/HW1/CO417-HW1/Memorial/memorial_tm_brighter.ppm
fi

exit 0;
