#!/bin/bash 

## HDR image cleanup
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
if [[ -f $AG/HW1/CO417-HW1/Memorial/memorial_gamma.pfm ]]; then
rm $AG/HW1/CO417-HW1/Memorial/memorial_gamma.pfm
fi

## IBL image cleanup
if [[ -f $AG/HW1/CO417-HW1/GraceCathedral/grace_relit.ppm ]]; then
rm $AG/HW1/CO417-HW1/GraceCathedral/grace_relit.ppm
fi
if [[ -f $AG/HW1/CO417-HW1/GraceCathedral/grace_relit.pfm ]]; then
rm $AG/HW1/CO417-HW1/GraceCathedral/grace_relit.pfm
fi

exit 0;