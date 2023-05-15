import CMovement from "../../lib/component/CMovement.js";
import CVec3 from "../../lib/geometry/CVec3.js";
import CObjBD from "../../lib/component/CObjBD.js";
import { CPaint3D } from "../../lib/component/CPaint.js";
import CVec4 from "../../lib/geometry/CVec4.js";
import CAnimation, { CClipCObjHD, CClipPRS, CClipColor, CClipPhysics } from "../../lib/component/CAnimation.js";
import CAniFlow from "../../lib/component/CAniFlow.js";
import CParticle, { CPTCComponent, CPTCShapeOut} from "../../lib/component/CPaticle.js";
import CFramework from "../../lib/util/CFramework.js";
import CMath from "../../lib/geometry/CMath.js";
import CRenderPass from "../../lib/render/CRenderPass.js";
import CRigidBody from "../../lib/component/CRigidBody.js";
import Df from "../../lib/basic/Df.js";
import CWatch from "../../lib/basic/CWatch.js";

export class CPTCFirework extends CPTCShapeOut {
	m_accMin;
	m_accMax;
	
    constructor() {
        super();
        this.m_accMin = 0;
        this.m_accMax = 0;
    }
    Process(_obj, _rb) {
        if (this.m_dirMax.equals(this.m_dirMin) == false) {
            var dirX = Math.random() * (this.m_dirMax.x - this.m_dirMin.x) + this.m_dirMin.x;
            var dirY = Math.random() * (this.m_dirMax.y - this.m_dirMin.y) + this.m_dirMin.y;
            var dirZ = Math.random() * (this.m_dirMax.z - this.m_dirMin.z) + this.m_dirMin.z;
            var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
            let movement = new CMovement("paticle", CMath.V3Nor(new CVec3(dirX, dirY, dirZ)), speed);
            movement.m_acc = Math.random() * (this.m_accMax - this.m_accMin) + this.m_accMin;
            _rb.Push(movement);
        }
        var posX = Math.random() * (this.m_posMax.x - this.m_posMin.x) + this.m_posMin.x;
        var posY = Math.random() * (this.m_posMax.y - this.m_posMin.y) + this.m_posMin.y;
        var posZ = Math.random() * (this.m_posMax.z - this.m_posMin.z) + this.m_posMin.z;
        _obj.SetPos(new CVec3(posX, posY, posZ));
    }
};


export default function Firework(_can, _pos)
{	
	//폭죽 원형
	let center = new CObjBD();
	center.SetSca(new CVec3(0.25, 0.125, 0.25));
	_pos.toCopy(center.GetPos());
	_can.Push(center);
	
	let center_pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	center_pt.SetRGBA(new CVec4(1,0,0,0));
	center.PushCComponent(center_pt);
	
	let part = new CObjBD();
	center.PushChilde(part);
	part.SetSca(new CVec3(1, 0.3, 1));
	part.SetPos(new CVec3(0, -130, 0));
	
	center_pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	center_pt.SetRGBA(new CVec4(1,1,-1,0));
	part.PushCComponent(center_pt);
	
	part = new CObjBD();
	center.PushChilde(part);
	part.SetSca(new CVec3(0.67, 0.3, 0.67));
	part.SetPos(new CVec3(0, 130, 0));
	
	center_pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	center_pt.SetRGBA(new CVec4(0,0,0,0));
	part.PushCComponent(center_pt);
	
	part = new CObjBD();
	center.PushChilde(part);
	part.SetSca(new CVec3(0.22, 0.3, 0.22));
	part.SetPos(new CVec3(0, 160, 0));
	
	center_pt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	center_pt.SetRGBA(new CVec4(0,0,0,0));
	part.PushCComponent(center_pt);
	
	let ani = new CAnimation();
	let clipPRS = new CClipPRS(0, 2000, center.GetPos().toCopy(), CMath.V3AddV3(center.GetPos(), new CVec3(0, 1000, 0)), 0);
	clipPRS.m_curve.m_type = 1;
	ani.Push(clipPRS);
	ani.Push(new CClipCObjHD(2000, true));
	
	center.PushCComponent(new CAniFlow(ani));
	
	FireWorkTrace(_can, center);
	FireWorkExplode(_can, center);
}

function FireWorkTrace(_can, _center) {
		//폭죽 원형이 움직일 때 반대방향으로 파티클
	let ptc = new CParticle();
	let ptcp = new CPTCComponent();
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	ptcp.m_comArr.push(ptt);
	
	let ani = new CAnimation();
	ani.Push(new CClipColor(0, 2000, new CVec4(1, 1, -1, 0), new CVec4(1, 0, 0, 0)));
	let clipPRSSca = new CClipPRS(0, 2000, new CVec3(0.1, 0.1, 0.1), new CVec3(0, 0, 0), 2);
	clipPRSSca.m_curve.m_type = 2;
	ani.Push(clipPRSSca);
	ani.Push(new CClipCObjHD(2000, true));
	ptcp.m_comArr.push(new CAniFlow(ani));
	ptc.m_cptcArr.push(ptcp);
	
	let shape = new CPTCShapeOut();
	shape.m_dirMin = new CVec3(-0.5, -1, -0.5);
	shape.m_dirMax = new CVec3(0,-1,0);
	shape.m_posMin = new CVec3(0, -60, 0);
	shape.m_posMax = new CVec3(0, -60, 0);
	ptc.m_shape = shape;
	
	ptc.m_target = _center;
	ptc.m_createTime = 200;
	ptc.m_createCount = 1;
	ptc.m_endTime = 2000;
	
	ani = new CAnimation();
	ani.Push(new CClipCObjHD(4000, true));
	ptc.PushCComponent(new CAniFlow(ani));
	_can.Push(ptc);
}

let color = new Map();
	color[0] = new CVec4(0.6,0.035,0.16,0); // 갈색
	color[1] = new CVec4(1,1,0,0); // 노랑
	color[2] = new CVec4(0.6,0.8,1,0); // 하늘
	color[3] = new CVec4(0,0.4,0.63,0); //진한 파랑
	color[4] = new CVec4(0.5,0.75,0.27,0); // 연두
	color[5] = new CVec4(0.3,0.21,0.6,0); //진녹색
	color[6] = new CVec4(0.54,0,1,0); //보라
	color[7] = new CVec4(0,0.5,1,0); //파랑

function FireWorkExplode(_can, _center) {
		//폭죽(4번에 걸쳐서)(초기속도가 점점 작아짐)
	for(let i = 0; i < 4; i++) {
		let ran = Math.random() * Math.PI;
		for(let j = 0; j < 4 - i; j++) {
			let ptc = new CParticle();
			let ptcp = new CPTCComponent();
			let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
			ptcp.m_comArr.push(ptt);
			
			let ani = new CAnimation();
			
			let initScale = Math.random() * (0.1 + 0.025 * i) + 0.025 * (4 - i);
			
			let prs = new CClipPRS(0, 2000, new CVec3(initScale, initScale, initScale), new CVec3(0, 0, 0), 2);
			prs.m_curve.m_type = 2;
			ani.Push(prs);
			ani.Push(new CClipPRS(0, 0, new CVec3(ran, ran, ran), new CVec3(ran, ran, ran), 1));
			for(let k = 0; k < 2; k++) {
				let rgbaClip =new CClipColor(1000 * k, 1000, color[i + k], color[i + k + 1]);
				rgbaClip.m_curve.m_type = 0;
				ani.Push(rgbaClip);
			}
			ani.Push(new CClipCObjHD(2000, true));
			ptcp.m_comArr.push(new CAniFlow(ani));
			ptc.m_cptcArr.push(ptcp);
			
			let shape = new CPTCFirework();
			shape.m_dirMin = new CVec3(-1,-1,-1);
			shape.m_dirMax = new CVec3(1,1,1);
			shape.m_posMin = new CVec3(-10, -10, -10);
			shape.m_posMax = new CVec3(10, 10, 10);
			shape.m_speedMin = 300 + 50 * j;
			shape.m_speedMax = 600 + 50 * j;
			shape.m_accMin = -1000;
			shape.m_accMax = -580;
			ptc.m_shape = shape;
			
			ptc.m_target = _center;
			ptc.m_createTime = 100 * (i + 1);
			ptc.m_createCount = 2;
			ptc.m_startTime = 2000 + 500 * i;
			ptc.m_endTime = 2000 + 500 * i + 300;
			
			ani = new CAnimation();
			ani.Push(new CClipCObjHD(2000 + 2000 * (i + 1), true));
			ptc.PushCComponent(new CAniFlow(ani));
			_can.Push(ptc);
		}
	}

	for(let i = 0; i < (Math.random() + 1) * 50; i++) {
		ExplodeCircle(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), 1000, 1000, 100, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 1000 * i);
		ExplodeCircle(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), 1000, 1000, 100, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 1000 * i);
		
		ExplodeCircle(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), 500, 1000, 100, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 1000 * i);
		ExplodeCircle(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), 1500, 1000, 100, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 1000 * i);

	}
	
	for(let i = 0; i < (Math.random() * 30); i++) {
		ExplodeSmoke(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), (Math.random() + 1) * 50, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 10000 + 5000 * i, 5000);
		ExplodeSmoke(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), (Math.random() + 1) * 50, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 10000 + 5000 * i, 5000);
		ExplodeSmoke(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), (Math.random() + 1) * 50, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 10000 + 5000 * i, 5000);
		ExplodeSmoke(_can, _center, new CVec3(Math.random(), Math.random(), Math.random()), (Math.random() + 1) * 50, new CVec4(Math.random(), 1, 1, 0), new CVec4(Math.random(), 1, 0, 0), 6000 + 10000 + 5000 * i, 5000);
	}
}

class CPTCSmoke extends CWatch {
	m_dir;
	m_totalObjNum;
	m_totalCycle;
	m_indexObj;
	m_indexCycle;
	m_speed;
	m_lifeMin;
	m_lifeMax;
	m_colorBefore;
	m_colorAfter;
	m_distance;
	m_particlesInOneFrame;
	m_indexParticles;
	m_scale;
	
	constructor() {
        super(...arguments);
        this.m_dir = new CVec3(0, 1, 0);
        this.m_totalObjNum = 30;
        this.m_totalCycle = 3;
        this.m_indexObj = 0;
        this.m_indexCycle = 0;
        this.m_speed = 30;
        this.m_lifeMin = 1000;
        this.m_lifeMax = 1500;
        this.m_colorBefore = new CVec4(0,0,0,0);
        this.m_colorAfter = new CVec4(1,1,1,0);
        this.m_distance = new CVec3(0,0,0);
        this.m_particlesInOneFrame = 1;
        this.m_indexParticles = 0;
        this.m_scale = 0.1;
    }
    Process(_obj, _rb) {
		if(this.m_indexParticles > this.m_particlesInOneFrame) {
			this.m_indexParticles = 0;
		} else {
			this.m_indexParticles++;
		}
		_obj.SetPos(CMath.V3AddV3(_obj.GetPos(), CMath.V3MulFloat(CMath.V3Nor(this.m_dir), this.m_distance / this.m_totalObjNum * this.m_indexParticles)));
		
		let objNumInOneCycle = this.m_totalObjNum / this.m_totalCycle;
		let yaxis = new CVec3(0, 1, 0);
		let vec = CMath.V3Cross(yaxis, this.m_dir);
		let multiplier = (this.m_indexCycle % 2 == 0)? 
			0 + this.m_indexObj / objNumInOneCycle :
			1 - this.m_indexObj / objNumInOneCycle;
		this.m_indexObj++;
		if(this.m_indexObj >= objNumInOneCycle) {
			this.m_indexObj = 0;
			this.m_indexCycle++;
		}
		if(this.m_indexCycle == this.m_totalCycle) {
			this.m_indexObj = 0;
			this.m_indexCycle = 0;
		}
		let angle = multiplier * Math.PI * 2;
		vec = RotateAroundAxis(vec, this.m_dir, angle);
		
		let movement = new CMovement("particle", vec, this.m_speed);
		movement.m_acc = -this.m_speed;
		movement.m_minPow = 5;
		_rb.Push(movement);
		
		let ani = new CAnimation();
		
		let life = Math.random() * (this.m_lifeMax - this.m_lifeMin) + this.m_lifeMin;
		ani.Push(new CClipCObjHD(life, true));
		
		let random = (Math.random() + 1) / 2;
		let mult = 1 - (this.m_indexObj + objNumInOneCycle * this.m_indexCycle) / this.m_totalObjNum;
		let clipDownScale = new CClipPRS(0, life * random, CMath.V3MulFloat(new CVec3(0.5,0.5,0.5), mult * mult * this.m_scale), CMath.V3MulFloat(new CVec3(1,1,1), mult * mult * this.m_scale), 2);
		ani.Push(clipDownScale);
		let clipUpScale = new CClipPRS(life * random, life * (1 - random), CMath.V3MulFloat(new CVec3(1,1,1), mult * mult * this.m_scale), new CVec3(0,0,0), 2);
		clipUpScale.m_curve.m_type = 2;
		ani.Push(clipUpScale);
		
		let clipColor = new CClipColor(0, life, this.m_colorBefore, this.m_colorAfter);
		clipColor.m_space = 2;
		ani.Push(clipColor);
		
		_obj.PushCComponent(new CAniFlow(ani));
    }
}

function ExplodeSmoke(_can, _center, _dir, _distance, _colorBefore, _colorAfter, _endTime, _lifeTime) {
	let obj = new CObjBD();
	
	let ani = new CAnimation();
	ani.Push(new CClipCObjHD(_endTime + _lifeTime, true));
	ani.Push(new CClipPRS(_endTime, _lifeTime, new CVec3(0,0,0), CMath.V3MulFloat(_dir, _distance), 0));
	obj.PushCComponent(new CAniFlow(ani));
	
	_center.PushChilde(obj);
	
	let ptc = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = false;
	rp.m_priority = Df.RenderPriority.Normal + 30;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	ptt.SetRenderPass(rp);
	ptcp.m_comArr.push(ptt);
	ptc.m_cptcArr.push(ptcp);
	
	let rb = new CRigidBody();
	rb.Push(new CMovement("gravity", new CVec3(0, -1, 0), 0.98));
	ptcp.m_comArr.push(rb);
	
	let lifeTime = _lifeTime;
	let shape = new CPTCSmoke();
	shape.m_dir = _dir;
	shape.m_totalObjNum = Math.random() * 100 + 100;
	shape.m_totalCycle = Math.random() * 5;
	shape.m_speed = Math.random() * 400 + 200;
	shape.m_lifeMin = lifeTime * 0.5;
	shape.m_lifeMax = lifeTime * 1.5;
	shape.m_scale = 0.04;
	shape.m_colorBefore = _colorBefore;
	shape.m_colorAfter = _colorAfter;
	shape.m_distance = _distance;
	shape.m_particlesInOneFrame = 1;
	ptc.m_shape = shape;
	
	ptc.m_target = obj;
	ptc.m_createCount = 1;
	ptc.m_createTime = 50;
	ptc.m_endTime = _endTime + 5000;
	ptc.m_startTime = _endTime;
	
	_can.Push(ptc);
}

//axis need to be normailzed first
function RotateAroundAxis(vec, axis, radian) {
	let cross = CMath.V3Cross(axis, vec);
	let f = CMath.V3MulFloat(axis, CMath.V3Dot(axis, vec));
	let s = CMath.V3MulFloat(CMath.V3Cross(cross, axis), Math.cos(radian));
	let t = CMath.V3MulFloat(cross, Math.sin(radian));
	
	return CMath.V3AddV3(CMath.V3AddV3(f, s), t);
}

class CPTCExplodeCircle extends CPTCFirework {
	m_axis;
	m_lifeMin;
	m_lifeMax;
	m_angleMin;
	m_angleMax;
	m_colorMin;
	m_colorMax;
	m_scaleMax;
	m_scaleMin;
	m_count;
	m_speedMax;
	m_speedMin;
	
	m_indexRot;
	constructor() {
        super();
        this.m_axis = new CVec3(0, 1, 0);
        this.m_lifeMin = 500;
        this.m_lifeMax = 1000;
        this.m_angleMin = 0;
		this.m_angleMax = 60;
        this.m_indexRot = 0;
        this.m_count = 100;
        this.m_scaleMax = 0.3;
		this.m_scaleMin = 0.1;
        this.m_colorMax = new CVec4(1,1,1,0);
        this.m_colorMin = new CVec4(0,0,0,0);
        this.m_speedMax = 0;
        this.m_speedMin = 0;
    }
    Process(_obj, _rb) {
		this.m_indexRot += Math.PI * 2 / this.m_count;
		this.m_axis = CMath.V3Nor(this.m_axis);
		let arbitraryVec = new CVec3(1, 0, 0);
		arbitraryVec = (arbitraryVec == this.m_axis)? new CVec3(0,1,0) : arbitraryVec;
		let randomForward = CMath.V3Cross(this.m_axis, arbitraryVec);
		let dir = RotateAroundAxis(randomForward, this.m_axis, this.m_indexRot);
		dir = CMath.V3Nor(dir);
        var speed = Math.random() * (this.m_speedMax - this.m_speedMin) + this.m_speedMin;
        let movement = new CMovement("paticle", dir, speed);
        movement.m_acc = Math.random() * (this.m_accMax - this.m_accMin) + this.m_accMin;
        movement.m_minPow = 10;
        _rb.Push(movement);
        
        var posX = Math.random() * (this.m_posMax.x - this.m_posMin.x) + this.m_posMin.x;
        var posY = Math.random() * (this.m_posMax.y - this.m_posMin.y) + this.m_posMin.y;
        var posZ = Math.random() * (this.m_posMax.z - this.m_posMin.z) + this.m_posMin.z;
        _obj.SetPos(new CVec3(posX, posY, posZ));
        
        let ani = new CAnimation();
        let life = Math.random() * (this.m_lifeMax - this.m_lifeMin) + this.m_lifeMin;
        ani.Push(new CClipCObjHD(life, true));
        let clipPhy = new CClipPhysics(0, life);
		clipPhy.m_rotUse = true;
		let rotX = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		let rotY = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		let rotZ = Math.random() * (this.m_angleMax - this.m_angleMin) + this.m_angleMin;
		clipPhy.m_rot = CMath.EulerToQut(new CVec3(rotX, rotY, rotZ));
		ani.Push(clipPhy);
		
		let colorBefore = CMath.V4MulFloat(CMath.V4SubV4(this.m_colorMax, this.m_colorMin), Math.random() * 0.5);
		colorBefore = CMath.V4AddV4(colorBefore, this.m_colorMin);
		let colorAfter = CMath.V4MulFloat(CMath.V4SubV4(this.m_colorMax, this.m_colorMin), (1 + Math.random()) / 2);
		colorAfter = CMath.V4AddV4(colorAfter, this.m_colorMin);
		let clipColor = new CClipColor(0, life, colorBefore, colorAfter);
		clipColor.m_space = 2;
		clipColor.m_curve.m_type = 0;
		ani.Push(clipColor);
		
		let scale = Math.random() * (this.m_scaleMax - this.m_scaleMin) + this.m_scaleMin;
		let clipScale = new CClipPRS(0, life, CMath.V3MulFloat(new CVec3(1.35,1.35, 1.35), scale), new CVec3(), 2);
		clipScale.m_curve.m_type = 3;
		ani.Push(clipScale);
        
        _obj.PushCComponent(new CAniFlow(ani));
    }
}

export function ExplodeCircle(_can, _obj, _axis, _radius, _life, _num, _colorMin, _colorMax, _startTime) {
	let Particle = new CParticle();
	let ptcp = new CPTCComponent();
	let rp = new CRenderPass();
	rp.m_depthTest = false;
	rp.m_priority = Df.RenderPriority.Normal + 20;
	rp.m_vf = "PreSimple";
	let ptt = new CPaint3D(CFramework.Main().Pal().GetBoxMesh());
	ptt.SetRenderPass(rp);
	ptcp.m_comArr.push(ptt);
	ptcp.m_rate = 4;
	
	let lifeTime = _life;
	
	let rb = new CRigidBody();
	rb.Push(new CMovement("gravity", new CVec3(0, -1, 0), 9.8));
	ptcp.m_comArr.push(rb);
	
	Particle.m_cptcArr.push(ptcp);
	
	let shape = new CPTCExplodeCircle();
	shape.m_accMin = -(_radius + 100) * 3;
	shape.m_accMax = -(_radius + 100) * 3;
	shape.m_speedMax = _radius + 100;
	shape.m_speedMin = _radius + 60;
	shape.m_lifeMax = lifeTime * 1.5;
	shape.m_lifeMin = lifeTime * 0.5;
	shape.m_angleMin = Math.PI * 0;
	shape.m_angleMax = Math.PI;
	shape.m_colorMin = _colorMin;
	shape.m_colorMax = _colorMax;
	shape.m_scaleMax = 0.04;
	shape.m_scaleMin = 0.02;
	shape.m_axis = _axis;
	shape.m_count = _num;
	
	Particle.m_shape = shape;
	Particle.m_createCount = _num;
	Particle.m_createTime = 100;
	Particle.m_endTime = _startTime + 100;
	Particle.m_startTime = _startTime;
	Particle.m_target = _obj;
	
	_can.Push(Particle);
}