from sphere import SphereRenderer
import imageIO

def main():
	renderer = SphereRenderer(511)
	latlongPath = '/Users/paul/github/hdr/GraceCathedral/grace_latlong.pfm'
	writedir = '/Users/paul/github/hdr/cw2/'
	graceLatlong = imageIO.loadPFMFile( latlongPath )
	sphereMapped_pfm = renderer.mapLatlong( graceLatlong )
	imageIO.writePPMFile( latlongPath + 'graceSphere.pfm', sphereMapped_pfm )
	sphereMapped_ppm = imageIO.toPPM( sphereMapped_pfm )
	imageIO.writePPMFile( latlongPath + 'graceSphere.ppm', sphereMapped_ppm )