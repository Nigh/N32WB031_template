import { defineConfig } from 'vite';
import reactRefresh from '@vitejs/plugin-react-refresh';
import WindiCSS from 'vite-plugin-windicss';
import { VitePWA } from 'vite-plugin-pwa'

// https://vitejs.dev/config/
export default defineConfig({
	plugins: [
		reactRefresh(),
		WindiCSS(),
		VitePWA({
			registerType: "autoUpdate",
			includeAssets: [
				"favicon.ico",
				"pwa-192x192.png",
				"pwa-512x512.png",
				"robots.txt",
			],
			manifest: {
				short_name: "N32WB-Dev",
				name: "N32WB-Dev-Tool",
				icons: [
					{
						src: "favicon.ico",
						type: "image/png",
						sizes: "32x32",
					},
					{
						src: "pwa-192x192.png",
						type: "image/png",
						sizes: "192x192",
					},
					{
						src: "pwa-512x512.png",
						type: "image/png",
						sizes: "512x512",
						purpose: "any maskable",
					},
				],
				id: "/n32wb_evb/",
				start_url: "/n32wb_evb/",
				scope: "/n32wb_evb/",
				orientation: "portrait",
				display: "standalone",
				theme_color: "#ffffff",
				background_color: "#000000",
			},
		}),
	],
	base: "./",
})
