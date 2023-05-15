import CMovement from "../../lib/component/CMovement.js";
import CVec3 from "../../lib/geometry/CVec3.js";
import CObjBD from "../../lib/component/CObjBD.js";
import { CPaint3D, CPaintTrail } from "../../lib/component/CPaint.js";
import CVec4 from "../../lib/geometry/CVec4.js";
import CAnimation, { CClipCObjHD, CClipPRS, CClipColor } from "../../lib/component/CAnimation.js";
import CAniFlow from "../../lib/component/CAniFlow.js";
import CParticle, { CPTCComponent } from "../../lib/component/CPaticle.js";
import CFramework from "../../lib/util/CFramework.js";
import CMath from "../../lib/geometry/CMath.js";
import CWatch from "../../lib/basic/CWatch.js";
import CRenderPass from "../../lib/render/CRenderPass.js";

export default function Explode(_can, _pos, _color) {
	let center = new CObjBD();
	_pos.toCopy(center.GetPos());
	center.SetSca(new CVec3(0.1, 0.1, 0.1));
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(2000, true));
	center.PushCComponent(new CAniFlow(ani));
	
	_can.Push(center);
	
	
	//터지는거
	Bomb(_can, center, _color);
	
	//튀어나오는거
	Debris(_can, center, 4);
}

export function ExplodeOneDir(_can, _pos, _color, _dir, _num, _lineNum = 1) {
		let center = new CObjBD();
	_pos.toCopy(center.GetPos());
	center.SetSca(new CVec3(0.1, 0.1, 0.1));
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(2000, true));
	center.PushCComponent(new CAniFlow(ani));
	
	_can.Push(center);
	
	
	//터지는거
	BombOneDir(_can, center, _color, _dir, _num, _lineNum);
	
	//튀어나오는거
	//Debris(_can, center, 4);
}

//axis need to be normailzed first
function RotateAroundAxis(vec, axis, radian) {
	let cross = CMath.V3Cross(axis, vec);
	let f = CMath.V3MulFloat(axis, CMath.V3Dot(axis, vec));
	let s = CMath.V3MulFloat(CMath.V3Cross(cross, axis), Math.cos(radian));
	let t = CMath.V3MulFloat(cross, Math.sin(radian));
	
	return CMath.V3AddV3(CMath.V3AddV3(f, s), t);
}

function LookRotation(_forward, _up) {
	_forward = CMath.V3Nor(_forward);
	
	let vec = CMath.V3Nor(_forward);
	let vec2 = CMath.V3Nor(CMath.V3Cross(_up, vec));
	let vec3 = CMath.V3Cross(vec, vec2);
	
	let m00 = vec2.x;
	let m01 = vec2.y;
	let m02 = vec2.z;
	let m10 = vec3.x;
	let m11 = vec3.y;
	let m12 = vec3.z;
	let m20 = vec.x;
	let m21 = vec.y;
	let m22 = vec.z;
	
	let num8 = (m00 + m11) + m22;
	let qut = new CVec4();
	if(num8 > 0) {
		let num = Math.sqrt(num8 + 1);
		qut.w = num * 0.5;
		num = 0.5 / num;
		qut.x = (m12 - m21) * num;
		qut.y = (m20 - m02) * num;
		qut.z = (m01 - m10) * num;
		return qut;
	}
	if((m00 >= m11) && (m00 >= m22)) {
		let num7 = Math.sqrt(((1 + m00) - m11) - m22);
		let num4 = 0.5 / num7;
		qut.x = 0.5 / num7;
		qut.y = (m01 + m10) * num4;
		qut.z = (m02 + m20) * num4;
		qut.w = (m12 - m21) * num4;
		return qut;
	}
	if(m11 > m22) {
		let num6 = Math.sqrt(((1 + m11) - m00) - m22);
		let num3 = 0.5 / num6;
		qut.x = (m10 + m01) * num3;
		qut.y = 0.5 * num6;
		qut.z = (m21 + m12) * num3;
		qut.w = (m20 - m02) * num3;
		return qut;
	}
	let num5 = Math.sqrt(((1 + m22) - m00) - m11);
	let num2 = 0.5 / num5;
	qut.x = (m20 + m02) * num2;
	qut.y = (m21 + m12) * num2;
	qut.z = 0.5 * num5;
	qut.w = (m01 - m10) * num2;
	return qut;
}

export class CPTCExplode extends CWatch {
	m_speedMin;
	m_speedMax;
	m_scaleMin;
	m_scaleMax;
	m_colorMin;
	m_colorMid;
	m_colorMax;
	m_lifeMin;
	m_lifeMax;
	m_numberOfObjInOneLine;
	m_numberOfLine;
	m_dyingTime;
	m_indexOfObj;
	m_indexOfLine;
	m_dir;
	m_qut;
	m_initDir;
    constructor() {
        super();
		this.m_speedMin = 0;
		this.m_speedMax = 100;
		this.m_scaleMin = 0.2;
		this.m_scaleMax = 0.5;
		this.m_colorMin = new CVec4(0,0,0,0);
		this.m_colorMid = new CVec4(0,0,0,0);
		this.m_colorMax = new CVec4(0,0,0,0);
		this.m_lifeMin = 800;
		this.m_lifeMax = 1000;
		this.m_numberOfObjInOneLine = 5;
		this.m_numberOfLine = 14;
		
		this.m_dyingTime = 500;
		this.m_indexOfObj = 0;
		this.m_indexOfLine = 0;
		this.m_dir = new Array();
		this.m_qut = new Array();
		
		this.m_initDir = null;
    }
    Process(_obj, _rb) {
		//속도
		let random = 0;
		let speed = this.m_speedMax;
		
		if(this.m_indexOfObj % 2 == 0) {
			if(this.m_indexOfObj != 0) {
				random = 0.5 - (this.m_indexOfObj - 2) / (this.m_numberOfObjInOneLine - 2) + Math.random();
				random = (random > 1)? 1 : (random < 0)? 0 : random;
				speed = this.m_speedMin + (this.m_speedMax - this.m_speedMin) * (random);
			}
		} else {
			random = Math.random();
			speed = this.m_speedMin + (this.m_speedMax - this.m_speedMin) * (random);
		}
		
		//방향 - 한 라인에 한번만 계산
		if(this.m_initDir == null) {
			if(this.m_dir.length <= this.m_indexOfLine) {
				this.m_dir.push(new CVec3(
					Math.cos(Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine),
					0,
					Math.sin(Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine)
				));
				this.m_qut.push(CMath.EulerToQut(new CVec3(
					0,
					-Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine,
					0
				)));
			}
		} else {
			//시작 direction이 있을 때
			let xAxis = new CVec3(1,0,0);
			if(xAxis.x == this.m_initDir.x && xAxis.y == this.m_initDir.y && xAxis.z == this.m_initDir.z) {
				this.m_dir.push(new CVec3(
					Math.cos(Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine),
					0,
					Math.sin(Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine)
				));
				this.m_qut.push(CMath.EulerToQut(new CVec3(
					0,
					-Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine,
					0
				)));
			} else {
				let upVec = CMath.V3Cross(this.m_initDir, xAxis);
				
				if(this.m_dir.length <= this.m_indexOfLine) {
					this.m_dir.push(CMath.V3Nor(RotateAroundAxis(this.m_initDir, upVec, Math.PI * 2 / this.m_numberOfLine * this.m_indexOfLine)));
					this.m_qut.push(LookRotation(this.m_dir[this.m_indexOfLine], upVec));
				}
			}
		}
		
		//무브먼트
		let movement = new CMovement("particle", this.m_dir[this.m_indexOfLine], speed);
		movement.m_acc = -this.m_speedMax * (1000 / this.m_lifeMin);
		movement.m_minPow = 20;
		_rb.Push(movement);
		
		//오브젝트 방향
		_obj.SetRot(CMath.QutMul(_obj.GetRot(), this.m_qut[this.m_indexOfLine]));
		
		//애니메이션
		let ani = new CAnimation();
		let scale = new CVec3(1,1,1);
		
		//가장 바깥쪽 오브젝트
		if(this.m_indexOfObj == 0) {
			let lifeTime = this.m_lifeMin + (this.m_lifeMax - this.m_lifeMin) * 0.2;
			
			let clipColor = new CClipColor(0, lifeTime, this.m_colorMid, this.m_colorMid);
			clipColor.m_space = 2;
			ani.Push(clipColor);
			
			let clipPRS = new CClipPRS(0, lifeTime, CMath.V3MulFloat(scale, this.m_scaleMax), CMath.V3MulFloat(scale, this.m_scaleMax), 2);
			ani.Push(clipPRS);
			
			let clipColor2 = new CClipColor(lifeTime, this.m_dyingTime, this.m_colorMid, this.m_colorMax);
			clipColor2.m_space = 2;
			clipColor2.m_curve.m_type = 1;
			ani.Push(clipColor2);
			
			let clipPRS2 = new CClipPRS(lifeTime, this.m_dyingTime, CMath.V3MulFloat(scale, this.m_scaleMax), CMath.V3MulFloat(scale, this.m_scaleMin), 2);
			clipPRS2.m_curve.m_type = 1;
			ani.Push(clipPRS2);
			
			ani.Push(new CClipCObjHD(lifeTime + this.m_dyingTime, true));
		} 
		//연기 먼 거리에 있는 것부터 사라지는 오브젝트
		else if(this.m_indexOfObj % 2 == 0){
			let lifeTime = this.m_lifeMin + (this.m_lifeMax - this.m_lifeMin) * random;
			
			let minColor = CMath.V4SubV4(this.m_colorMid, this.m_colorMin);
			minColor = CMath.V4MulFloat(minColor, (1 - random));
			minColor = CMath.V4AddV4(minColor, this.m_colorMin);
			let clipColor = new CClipColor(0, lifeTime, minColor, this.m_colorMax);
			clipColor.m_curve.m_type = 3;
			clipColor.m_space = 2;
			ani.Push(clipColor);
			
			let minScale = this.m_scaleMin + (this.m_scaleMax - this.m_scaleMin) * (1 - random);
			let maxScale = this.m_scaleMax;
			let clipPRS = new CClipPRS(0, lifeTime, CMath.V3MulFloat(scale, minScale), CMath.V3MulFloat(scale, maxScale), 2);
			clipPRS.m_curve.m_type = 3;
			ani.Push(clipPRS);
			
			let clipColor2 = new CClipColor(lifeTime, this.m_dyingTime, this.m_colorMax, this.m_colorMax);
			clipColor2.m_curve.m_type = 1;
			clipColor2.m_space = 2;
			ani.Push(clipColor2);
			
			let clipPRS2 = new CClipPRS(lifeTime, this.m_dyingTime, CMath.V3MulFloat(scale, maxScale), CMath.V3MulFloat(scale, this.m_scaleMin), 2);
			clipPRS2.m_curve.m_type = 1;
			ani.Push(clipPRS2);
			
			
			ani.Push(new CClipCObjHD(lifeTime + this.m_dyingTime, true));
		}
		//연기 - 랜덤하게 사라지는 오브젝트
		else {
			let lifeTime = this.m_lifeMin + (this.m_lifeMax - this.m_lifeMin) * random;
			
			let minColor = CMath.V4SubV4(this.m_colorMid, this.m_colorMin);
			minColor = CMath.V4MulFloat(minColor, (1 - random));
			minColor = CMath.V4AddV4(minColor, this.m_colorMin);
			let clipColor = new CClipColor(0, lifeTime, minColor, this.m_colorMid);
			clipColor.m_curve.m_type = 2;
			clipColor.m_space = 2;
			ani.Push(clipColor);
			
			let minScale = this.m_scaleMin + (this.m_scaleMax - this.m_scaleMin) * (1 - random);
			let maxScale = this.m_scaleMax;
			let clipPRS = new CClipPRS(0, lifeTime, CMath.V3MulFloat(scale, minScale), CMath.V3MulFloat(scale, maxScale), 2);
			clipPRS.m_curve.m_type = 2;
			ani.Push(clipPRS);
			
			let clipColor2 = new CClipColor(lifeTime, this.m_dyingTime, this.m_colorMid, this.m_colorMax);
			clipColor2.m_curve.m_type = 1;
			clipColor2.m_space = 2;
			ani.Push(clipColor2);
			
			let clipPRS2 = new CClipPRS(lifeTime, this.m_dyingTime, CMath.V3MulFloat(scale, maxScale), CMath.V3MulFloat(scale, this.m_scaleMin), 2);
			clipPRS2.m_curve.m_type = 1;
			ani.Push(clipPRS2);
			
			ani.Push(new CClipCObjHD(lifeTime + this.m_dyingTime, true));
		}
		_obj.PushCComponent(new CAniFlow(ani));
		
		//index
		if(this.m_indexOfLine >= this.m_numberOfLine - 1) {
			if(this.m_indexOfObj >= this.m_numberOfObjInOneLine - 1) {
				this.m_indexOfLine = 0;
				this.m_indexOfObj = 0;
				this.m_dir = null;
				this.m_qut = null;
				return;
			}
			this.m_indexOfObj++;
			this.m_indexOfLine = 0;
		} else {
			this.m_indexOfLine++;
		}
    }
}

let beforeExplodeColor = new CVec4(0, 0.78, 1, 0);
let midExplodeColor = new CVec4(0, 0.78, 0.5, 0);
let afterExplodeColor = new CVec4(0, 0.78, 0.2, -0.1);

function Bomb(_can, _center, _color) {
	let smoke = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh(), rp);
	ptcp.m_comArr.push(ptt);
	smoke.m_cptcArr.push(ptcp);
	
	let shape = new CPTCExplode();
	shape.m_numberOfLine = 14;
	shape.m_numberOfObjInOneLine = 15;
	shape.m_speedMax = 750;
	shape.m_speedMin = 250;
	shape.m_colorMin = _color;
	let mid = _color.toCopy();
	mid.z /= 2;
	shape.m_colorMid = mid;
	let max = _color.toCopy();
	max.z /= 5;
	max.w = -0.2;
	shape.m_colorMax = max;
	shape.m_lifeMax = 1000;
	shape.m_lifeMin = 200;
	shape.m_scaleMax = 0.07;
	shape.m_scaleMin = 0.005;
	smoke.m_shape = shape;
	
	smoke.m_target = _center;
	smoke.m_createTime = 100;
	smoke.m_createCount = shape.m_numberOfObjInOneLine * shape.m_numberOfLine;
	smoke.m_endTime = 100;
	
	_can.Push(smoke);
}

export function BombOneDir(_can, _center, _color, _dir, _num, _lineNum) {
	let smoke = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = true;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh(), rp);
	ptcp.m_comArr.push(ptt);
	smoke.m_cptcArr.push(ptcp);
	
	let shape = new CPTCExplode();
	shape.m_numberOfLine = _lineNum;
	shape.m_numberOfObjInOneLine = _num;
	shape.m_speedMax = 750;
	shape.m_speedMin = 250;
	shape.m_colorMin = _color;
	let mid = _color.toCopy();
	mid.z /= 2;
	shape.m_colorMid = mid;
	let max = _color.toCopy();
	max.z /= 5;
	max.w = -0.2;
	shape.m_colorMax = max;
	shape.m_lifeMax = 1000;
	shape.m_lifeMin = 200;
	shape.m_scaleMax = 0.07;
	shape.m_scaleMin = 0.005;
	shape.m_initDir = _dir;
	smoke.m_shape = shape;
	
	smoke.m_target = _center;
	smoke.m_createTime = 100;
	smoke.m_createCount = shape.m_numberOfObjInOneLine * shape.m_numberOfLine;
	smoke.m_endTime = 100;
	
	_can.Push(smoke);
}

export class CPTCDebrisEffect extends CWatch {	
	m_life;
	m_dirMin;
	m_dirMax;
	m_speedMin;
	m_speedMax;
	m_endTime;
	m_particleColor;
	m_scaMax;
	m_scaMin;
	m_time;
	m_particleDisappearTime;
	
    constructor() {
        super();
        this.m_life = 1000; // 파편의 유지시간
        this.m_dirMin = new CVec3(-1, 0, 0);
        this.m_dirMax = new CVec3(1, 1, 0);
        this.m_speedMin = 100; //속도
        this.m_speedMax = 100; //속도
        this.m_endTime = 100; //몇 초에 한번씩 생성하는지
        this.m_particleColor = new CVec4(0,0,0,0);
        this.m_scaMax = 0.3;
        this.m_scaMin = 0.1;
        this.m_time = 300; //잔상의 유지시간
        this.m_particleDisappearTime = 100; //오브젝트가 사라진 후 잔상이 사라질 때 까지의 시간
    }
    Process(_obj, _rb) {
        if (this.m_dirMax.equals(this.m_dirMin) == false) {
            var dirX = Math.random() * (this.m_dirMax.x - this.m_dirMin.x) + this.m_dirMin.x;
            var dirY = Math.random() * (this.m_dirMax.y - this.m_dirMin.y) + this.m_dirMin.y;
            var dirZ = Math.random() * (this.m_dirMax.z - this.m_dirMin.z) + this.m_dirMin.z;
            var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
            let norDir = CMath.V3Nor(new CVec3(dirX, dirY, dirZ));
            let move = new CMovement("paticle", norDir, speed);
            move.m_acc = -speed / (this.m_life / 1000);
            _rb.Push(move);
            
            //gravity
            _rb.Push(new CMovement("gravity", new CVec3(0, -1, 0), 100));
            
            let rot = new CVec3(Math.atan(dirZ / dirX), Math.atan(dirY / dirX), Math.atan(dirZ / dirY));
            _obj.SetRot(rot);
        }
        
        let pt = new CPaintTrail(CFramework.Main().Pal().GetBlackTex());
        pt.SetRGBA(this.m_particleColor);
        pt.m_lastSmall = true;
        pt.m_len = _obj.GetSca().x;
        pt.m_startTime = this.m_life;
        pt.m_lastHide = false;
        pt.m_endTime = this.m_particleDisappearTime;
        _obj.PushCComponent(pt);
    }
}

function Debris(_can, _center, _num) {
	//잔해
	let debris = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = false;
	rp.m_vf = "PreSimple";
	
	let pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	pt.SetRenderPass(rp);
	pt.SetRGBA(new CVec4(1,1,1,0));
	ptcp.m_comArr.push(pt);
	debris.m_cptcArr.push(ptcp);
	
	let ani = new CAnimation();
	let lifeTime = 1000;
	let lastTimeForTrailDisappear = 200;
	ani.Push(new CClipCObjHD(lifeTime + lastTimeForTrailDisappear, true));
	let scaMax = 0.01, scaMin = 0.005;
	let scaRan = CMath.V3MulFloat(new CVec3(1, 1, 1), Math.random() * (scaMax - scaMin) + scaMin);
	let clipPRS = new CClipPRS(0, lifeTime, scaRan, new CVec3(0, 0, 0), 2);
	clipPRS.m_curve.m_type = 2;
	ani.Push(clipPRS);
	let clipColor = new CClipColor(0, lifeTime, new CVec4(1,1,1,0), new CVec4(1,1,1,-1));
	clipColor.m_curve.m_type = 2;
	ani.Push(clipColor);
	ptcp.m_comArr.push(new CAniFlow(ani));
	
	let shape2 = new CPTCDebrisEffect();
	shape2.m_dirMax = new CVec3(1, 1, 1);
	shape2.m_dirMin = new CVec3(-1, 1, -1);
	shape2.m_speedMin = 250;
	shape2.m_speedMax = 750;
	shape2.m_life = lifeTime + lastTimeForTrailDisappear;
	shape2.m_particleColor = beforeExplodeColor;
	shape2.m_particleDisappearTime = lastTimeForTrailDisappear;
	debris.m_shape = shape2;
	
	debris.m_target = _center;
	debris.m_createTime = 100;
	debris.m_createCount = _num;
	debris.m_endTime = 100;
	
	_can.Push(debris);
}